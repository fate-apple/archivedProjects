#if (OBI_BURST && OBI_MATHEMATICS && OBI_COLLECTIONS)
using UnityEngine;
using Unity.Jobs;
using Unity.Collections;
using Unity.Collections.LowLevel.Unsafe;
using Unity.Mathematics;
using Unity.Burst;
using System.Collections;
using System.Collections.Generic;
using CSparse.Double;

namespace Obi
{
    using SparseMatrix = MathNet.Numerics.LinearAlgebra.Double.SparseMatrix;
    using Vec = MathNet.Numerics.LinearAlgebra.Vector<double>;
    public class BurstPDBendTwistConstraintsBatch : BurstConstraintsBatchImpl, IPDBendTwistConstraintsBatchImpl
    {
        
        private NativeArray<int> orientationIndices;
        private NativeArray<quaternion> restDarboux;
        private NativeArray<float3> stiffnesses;
        private NativeArray<float2> plasticity;
        
      
        private List<SparseMatrix> QuatRhsCached;
        private List<SparseMatrix> QuatLhsCached;
        public CRProperty crProperty;
        private NativeArray<float> WeightCoef;
        private NativeArray<float> restLengths;
        private NativeArray<quaternion> result;

        public BurstPDBendTwistConstraintsBatch(BurstPDBendTwistConstraints constraints)
        {
            m_Constraints = constraints;
            m_ConstraintType = Oni.ConstraintType.PDBendTwist;
        }

        public void SetBendTwistConstraints(ObiNativeIntList orientationIndices, ObiNativeFloatList restLengths, ObiNativeQuaternionList restDarboux, ObiNativeVector3List stiffnesses, ObiNativeVector2List plasticity, ObiNativeFloatList lambdas, int count)
        {
            this.orientationIndices = orientationIndices.AsNativeArray<int>();
            this.restDarboux = restDarboux.AsNativeArray<quaternion>();
            this.stiffnesses = stiffnesses.AsNativeArray<float3>();
            this.plasticity = plasticity.AsNativeArray<float2>();
            this.lambdas = lambdas.AsNativeArray<float>();
            this.restLengths = restLengths.AsNativeArray<float>();
            m_ConstraintCount = count;
            this.crProperty = solverImplementation.abstraction.PDCRProperty ;
            this.WeightCoef = new NativeArray<float>(this.restLengths,Allocator.Persistent);
            this.result = new NativeArray<quaternion>(this.m_ConstraintCount,Allocator.Persistent);
            InitMatrixCached();
        }

        public void InitMatrixCached()
        {
            var QuatA = SparseMatrix.CreateIdentity(8);
            var QuatSelectionMatrix = SparseMatrix.Create(8, (solverImplementation.PlainQuat.Count),0);
            QuatRhsCached = new List<SparseMatrix>();
            QuatLhsCached = new List<SparseMatrix>();
            for (int i = 0; i < m_ConstraintCount; i++)
            {
                QuatSelectionMatrix.Clear();
                int q1 = orientationIndices[i * 2];
                int q2 = orientationIndices[i * 2 + 1];
                WeightCoef[i] = 2*Mathf.PI*crProperty.G* math.pow( crProperty.R,4)/restLengths[i];
                QuatSelectionMatrix[0, 4 * q1] = 1;
                QuatSelectionMatrix[1, 4 * q1+1] = 1;
                QuatSelectionMatrix[2, 4 * q1+2] = 1;
                QuatSelectionMatrix[3, 4 * q1+3] = 1;
                QuatSelectionMatrix[4, 4 * q2] = 1;
                QuatSelectionMatrix[5, 4 * q2+1] = 1;
                QuatSelectionMatrix[6, 4 * q2+2] = 1;
                QuatSelectionMatrix[7, 4 * q2+3] = 1;
                
                QuatRhsCached.Add( WeightCoef[i] * QuatSelectionMatrix.Transpose() * QuatA.Transpose() as SparseMatrix);
                QuatLhsCached.Add( WeightCoef[i] * QuatSelectionMatrix.Transpose() * QuatA.Transpose()*QuatA*QuatSelectionMatrix as SparseMatrix);
                if (Global.debugMode)
                {
                    var debug0 = QuatSelectionMatrix.Transpose().ToArray();
                    debug0 = QuatA.Transpose().ToArray();
                    var debug1 = ((SparseMatrix)(WeightCoef[i] * QuatSelectionMatrix.Transpose() * QuatA.Transpose()))
                        .ToArray();
                    var debug2 =
                        ((SparseMatrix)(WeightCoef[i] * QuatSelectionMatrix.Transpose() * QuatA.Transpose() * QuatA *
                                        QuatSelectionMatrix)).ToArray();
                }
            }
        }
        
        public override JobHandle RebuildPDMatrix(JobHandle inputDeps)
        {
            for (int i = 0; i <m_ConstraintCount;i++)
            {
                if (Global.debugMode)
                {
                    var debug1 = QuatLhsCached[i].ToArray();
                    var debug4 = QuatRhsCached[i].ToArray();
                    var debug5 = solverImplementation.CRQuatLhs.ToArray();
                }
                solverImplementation.CRQuatLhs +=  QuatLhsCached[i];
            }
            return inputDeps;
        }
        public override JobHandle Evaluate(JobHandle inputDeps, float stepTime, float substepTime, int substeps)
        {
            
            if (Global.debugMode)
            {
                var orientations = solverImplementation.orientations;
                for (int i = 0; i < m_ConstraintCount; i++)
                {
                    int q1 = orientationIndices[i * 2];
                    int q2 = orientationIndices[i * 2 + 1];
                    
                    var quat1 = orientations[q1];
                    var quat2 = orientations[q2];
                    var curv = math.slerp(quat1, quat2, 0.5f);

                    result[i] = math.normalize(curv);
                }
               
                return inputDeps;
            }
            
            var projectConstraints = new BendTwistConstraintsBatchJob()
            {
                orientationIndices = orientationIndices,
                restDarboux = restDarboux,
                stiffnesses = stiffnesses,
                plasticity = plasticity,
                lambdas = lambdas.Reinterpret<float, float3>(),
            
                orientations = solverImplementation.orientations,
                invRotationalMasses = solverImplementation.invRotationalMasses,
            
                orientationDeltas = solverImplementation.orientationDeltas,
                orientationCounts = solverImplementation.orientationConstraintCounts ,
                result = result,
            
                deltaTime = substepTime
            };
            
            return projectConstraints.Schedule(m_ConstraintCount, 32, inputDeps);
        }

        public override JobHandle Apply(JobHandle inputDeps, float substepTime)
        {
            UnityEngine.Profiling.Profiler.BeginSample("BTApply");
            var parameters = solverAbstraction.GetConstraintParameters(m_ConstraintType);
            inputDeps.Complete();
            var vQuatDelta = Vec.Build.Dense(8);
            for (int i = 0; i < m_ConstraintCount; i++)
            {
                if (Global.debugMode)
                {
                    var debug2 = QuatRhsCached[i].ToArray();
                }

                for (int j = 0; j < 4; j++)
                {
                    vQuatDelta[j] = result[i].value[(j + 3) % 4];
                    vQuatDelta[j+4] = result[i].value[(j + 3) % 4];
                }
                var dQuatRhs = QuatRhsCached[i] * vQuatDelta;
                solverImplementation.CRQuatRhs += dQuatRhs;
            }
            UnityEngine.Profiling.Profiler.EndSample();
            return inputDeps;
            // var applyConstraints = new ApplyBendTwistConstraintsBatchJob()
            // {
            //     orientationIndices = orientationIndices,
            //
            //     orientations = solverImplementation.orientations,
            //     orientationDeltas = solverImplementation.orientationDeltas,
            //     orientationCounts = solverImplementation.orientationConstraintCounts,
            //
            //     sorFactor = parameters.SORFactor
            // };
            // return applyConstraints.Schedule(m_ConstraintCount, 64, inputDeps);
        }

        [BurstCompile]
        public struct BendTwistConstraintsBatchJob : IJobParallelFor
        {
            [ReadOnly] public NativeArray<int> orientationIndices;
            [ReadOnly] public NativeArray<float3> stiffnesses;
            [ReadOnly] public NativeArray<float2> plasticity;
            public NativeArray<quaternion> restDarboux;
            public NativeArray<float3> lambdas;

            [ReadOnly] public NativeArray<quaternion> orientations;
            [ReadOnly] public NativeArray<float> invRotationalMasses;

            [NativeDisableContainerSafetyRestriction][NativeDisableParallelForRestriction] public NativeArray<quaternion> orientationDeltas;
            [NativeDisableContainerSafetyRestriction][NativeDisableParallelForRestriction] public NativeArray<int> orientationCounts;
            [NativeDisableContainerSafetyRestriction][NativeDisableParallelForRestriction] public NativeArray<quaternion> result;

            [ReadOnly] public float deltaTime;

            public void Execute(int i)
            {
                int q1 = orientationIndices[i * 2];
                int q2 = orientationIndices[i * 2 + 1];

                float w1 = invRotationalMasses[q1];
                float w2 = invRotationalMasses[q2];

                var quat1 = orientations[q1];
                var quat2 = orientations[q2];
                var curv = math.slerp(quat1, quat2, 0.5f);

                result[i] = curv;
            }
        }

        [BurstCompile]
        public struct ApplyBendTwistConstraintsBatchJob : IJobParallelFor
        {
            [ReadOnly] public NativeArray<int> orientationIndices;
            [ReadOnly] public float sorFactor;

            [NativeDisableContainerSafetyRestriction] [NativeDisableParallelForRestriction] public NativeArray<quaternion> orientations;
            [NativeDisableContainerSafetyRestriction] [NativeDisableParallelForRestriction] public NativeArray<quaternion> orientationDeltas;
            [NativeDisableContainerSafetyRestriction] [NativeDisableParallelForRestriction] public NativeArray<int> orientationCounts;

            public void Execute(int i)
            {
                int p1 = orientationIndices[i * 2];
                int p2 = orientationIndices[i * 2 + 1];

                if (orientationCounts[p1] > 0)
                {
                    quaternion q = orientations[p1];
                    q.value += orientationDeltas[p1].value * sorFactor / orientationCounts[p1];
                    orientations[p1] = math.normalize(q);

                    orientationDeltas[p1] = new quaternion(0, 0, 0, 0);
                    orientationCounts[p1] = 0;
                }

                if (orientationCounts[p2] > 0)
                {
                    quaternion q = orientations[p2];
                    q.value += orientationDeltas[p2].value * sorFactor / orientationCounts[p2];
                    orientations[p2] = math.normalize(q);

                    orientationDeltas[p2] = new quaternion(0, 0, 0, 0);
                    orientationCounts[p2] = 0;
                }
            }

        }
    }
}
#endif