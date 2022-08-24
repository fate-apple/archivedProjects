#if (OBI_BURST && OBI_MATHEMATICS && OBI_COLLECTIONS)
using Unity.Jobs;
using Unity.Collections;
using Unity.Collections.LowLevel.Unsafe;
using Unity.Mathematics;
using Unity.Burst;
using System.Collections;
using System.Collections.Generic;

using JetBrains.Annotations;
using Quaternion = UnityEngine.Quaternion;
using Newtonsoft.Json;
using System.IO;
//using Extreme.Mathematics;
using MathNet.Numerics;
using UnityEngine;


namespace Obi
{
    // using SparseMatrix = Extreme.Mathematics.LinearAlgebra.SparseMatrix<float>;
    // using Vec = Extreme.Mathematics.Vector<float>;
    using SparseMatrix = MathNet.Numerics.LinearAlgebra.Double.SparseMatrix;
    using Vec = MathNet.Numerics.LinearAlgebra.Vector<double>;

    public class BurstPDStretchShearConstraintsBatch : BurstConstraintsBatchImpl, IStretchShearConstraintsBatchImpl
    {
        
        private NativeArray<int> orientationIndices;
        private NativeArray<float> restLengths;
        private NativeArray<quaternion> restOrientations;
        private NativeArray<float3> stiffnesses;

        private List<SparseMatrix> PosRhsCached;
        private List<SparseMatrix> QuatRhsCached;
        private List<SparseMatrix> PosLhsCached;
        private List<SparseMatrix> QuatLhsCached;
        public CRProperty crProperty;
        private NativeArray<float> WeightCoef;

        public BurstPDStretchShearConstraintsBatch(BurstPDStretchShearConstraints constraints)
        {
            m_Constraints = constraints;
            m_ConstraintType = Oni.ConstraintType.PDStretchShear;
        }

        public void SetStretchShearConstraints(ObiNativeIntList particleIndices, ObiNativeIntList orientationIndices, ObiNativeFloatList restLengths, ObiNativeQuaternionList restOrientations, ObiNativeVector3List stiffnesses, ObiNativeFloatList lambdas, int count)
        {
            this.particleIndices = particleIndices.AsNativeArray<int>();
            this.orientationIndices = orientationIndices.AsNativeArray<int>();
            this.restLengths = restLengths.AsNativeArray<float>();
            this.restOrientations = restOrientations.AsNativeArray<quaternion>();
            this.stiffnesses = stiffnesses.AsNativeArray<float3>();
            this.lambdas = lambdas.AsNativeArray<float>();
            this.WeightCoef = new NativeArray<float>(this.restLengths,Allocator.Persistent);
            this.crProperty = solverImplementation.abstraction.PDCRProperty ;
            m_ConstraintCount = count;
            InitMatrixCached();
        }

        public void InitMatrixCached()
        {
             
            // var QuatA = Matrix.CreateSparse<float>(4,4);
            // var PosA =Matrix.CreateSparse<float>( 3, 6,1/3);
            // var PosSelectionMatrix = Matrix.CreateSparse<float>(6, 3*solverImplementation.positions.Length,1/3*solverImplementation.positions.Length);
            // var QuatSelectionMatrix = Matrix.CreateSparse<float>(4, 4*solverImplementation.orientations.Length,1/4*solverImplementation.orientations.Length);
            var QuatA = SparseMatrix.CreateIdentity(4);
            var PosA =SparseMatrix.Create( 3, 6,0);
            SparseMatrix PosSelectionMatrix = SparseMatrix.Create( 6, 3*solverImplementation.positions.Length,0);
            var QuatSelectionMatrix = SparseMatrix.Create(4, solverImplementation.PlainQuat.Count,0);
            PosRhsCached = new List<SparseMatrix>();
            QuatRhsCached = new List<SparseMatrix>();
            PosLhsCached = new List<SparseMatrix>();
            QuatLhsCached = new List<SparseMatrix>();
            if (Global.uniformSampled)
            {
                for (int i = 0; i < m_ConstraintCount; i++)
                {
                    restLengths[i] = crProperty.restLength;
                }
            }
            
            for (int i = 0; i < m_ConstraintCount; i++)
            {
                // PosA.SetToZero();
                // PosSelectionMatrix.SetToZero();
                // QuatSelectionMatrix.SetToZero();
                PosA.Clear();
                PosSelectionMatrix.Clear();
                QuatSelectionMatrix.Clear();
                
                int p1 = particleIndices[i * 2];
                int p2 = particleIndices[i * 2 + 1];
                int q = orientationIndices[i];
                //TODO
                WeightCoef[i] *= crProperty.E*crProperty.A*restLengths[i];
                float x = 1/restLengths[i];
                PosA[0, 0] = x;
                PosA[1, 1] = x;
                PosA[2, 2] = x;
                PosA[0, 3] = -x;
                PosA[1, 4] = -x;
                PosA[2, 5] = -x;
                PosSelectionMatrix[0, 3 * p2] = 1;
                PosSelectionMatrix[1, 3 * p2+1] = 1;
                PosSelectionMatrix[2, 3 * p2+2] = 1;
                PosSelectionMatrix[3, 3 * p1] = 1;
                PosSelectionMatrix[4, 3 * p1+1] = 1;
                PosSelectionMatrix[5, 3 * p1+2] = 1;
                
                QuatSelectionMatrix[0, 4 * q] = 1;
                QuatSelectionMatrix[1, 4 * q+1] = 1;
                QuatSelectionMatrix[2, 4 * q+2] = 1;
                QuatSelectionMatrix[3, 4 * q+3] = 1;
                // PosRhsCached.Add( WeightCoef[i] * PosSelectionMatrix.Transpose() * PosA.Transpose() as SparseMatrix);
                // QuatRhsCached.Add( WeightCoef[i] * QuatSelectionMatrix.Transpose() * QuatA.Transpose() as SparseMatrix);
                // solverImplementation.CRPosLhs.AddInPlace( WeightCoef[i]*PosSelectionMatrix.Transpose()*PosA.Transpose()*PosA*PosSelectionMatrix as SparseMatrix);
                // solverImplementation.CRQuatLhs.AddInPlace( WeightCoef[i]*QuatSelectionMatrix.Transpose()*QuatA.Transpose()*QuatA*QuatSelectionMatrix);
              
                PosRhsCached.Add( WeightCoef[i] * PosSelectionMatrix.Transpose() * PosA.Transpose() as SparseMatrix);
                QuatRhsCached.Add( WeightCoef[i] * QuatSelectionMatrix.Transpose() * QuatA.Transpose() as SparseMatrix);
                PosLhsCached.Add( WeightCoef[i] * PosSelectionMatrix.Transpose() * PosA.Transpose()*PosA*PosSelectionMatrix as SparseMatrix);
                QuatLhsCached.Add( WeightCoef[i] * QuatSelectionMatrix.Transpose() * QuatA.Transpose()*QuatA*QuatSelectionMatrix as SparseMatrix);
                //solverImplementation.CRPosLhs += WeightCoef[i]*PosSelectionMatrix.Transpose()*PosA.Transpose()*PosA*PosSelectionMatrix as SparseMatrix;
                //solverImplementation.CRQuatLhs+= WeightCoef[i]*QuatSelectionMatrix.Transpose()*QuatA.Transpose()*QuatA*QuatSelectionMatrix as SparseMatrix;
            }
        }
        public override JobHandle RebuildPDMatrix(JobHandle inputDeps)
        {
            for (int i = 0; i <m_ConstraintCount;i++)
            {
                if (Global.debugMode)
                {
                    var debug0 = PosLhsCached[i].ToArray();
                    var debug1 = QuatLhsCached[i].ToArray();
                    var debug2 = PosRhsCached[i].ToArray();
                    var debug3 = solverImplementation.CRPosLhs.ToArray();
                    var debug4 = QuatRhsCached[i].ToArray();
                    var debug5 = solverImplementation.CRQuatLhs.ToArray();
                }
                solverImplementation.CRPosLhs +=  PosLhsCached[i];
                solverImplementation.CRQuatLhs +=  QuatLhsCached[i];
            }
            return inputDeps;
        }
        public void  MultiplyLJ(ref  NativeArray<float4>LJ)
        {
            for (int i = 0; i < restLengths.Length;i++)
            {
                int q = orientationIndices[i];
                LJ[q] *= restLengths[i];
            }
        }
        
        //compute projection
        public override JobHandle Evaluate(JobHandle inputDeps, float stepTime, float substepTime, int substeps)
        {
            var projectConstraints = new StretchShearConstraintsBatchJob()
            {
                particleIndices = particleIndices,
                orientationIndices = orientationIndices,
                restLengths = restLengths,
                restOrientations = restOrientations,
               

                positions = solverImplementation.positions,
                orientations = solverImplementation.orientations,
                invMasses = solverImplementation.invMasses,
                invRotationalMasses = solverImplementation.invRotationalMasses,

                PosProject = solverImplementation.positionDeltas,
                QuatProject = solverImplementation.orientationDeltas,
              

                deltaTimeSqr = substepTime * substepTime
            };
            //TODO:sjw debug use
            // inputDeps.Complete();
            // for (int i = 0; i <m_ConstraintCount;i++)
            // {
            //     int p1 = particleIndices[i * 2];
            //     int p2 = particleIndices[i * 2 + 1];
            //     int q = orientationIndices[i];
            //     
            //     float3 delta = (solverImplementation.positions[p2] - solverImplementation.positions[p1]).xyz;
            //
            //     //float3 e = math.rotate(restOrientations[i],new float3(0, 0, 1));
            //     quaternion basis = math.mul(solverImplementation.orientations[q],restOrientations[i]);
            //
            //     float3 d3 = math.rotate(basis, new float3(0, 0, 1));
            //    
            //
            //     var diff_un = Quaternion.FromToRotation(d3, delta);
            //     diff_un = diff_un.normalized;
            //     var un_star = diff_un * basis;
            //      un_star = diff_un * solverImplementation.orientations[q];
            //
            //     //used in re-compute rhs. should be index of constrain but both two batches use index:0-m_ConstraintCount, so use index of orientation instead
            //     //remember use same index when calculate Rhs
            //     solverImplementation.positionDeltas[q] = new float4(d3,0);
            //     solverImplementation.orientationDeltas[q] = un_star;
            // }

            return projectConstraints.Schedule(m_ConstraintCount, 32, inputDeps);
        }

       
        //add item with projection to Rhs
        public override JobHandle Apply(JobHandle inputDeps, float substepTime)
        {
            inputDeps.Complete();
            UnityEngine.Profiling.Profiler.BeginSample("SSApply");
            for (int i = 0; i < m_ConstraintCount; i++)
            {
                // var vPosDelta =  Vector.Create<float>(
                //     new float[]{solverImplementation.positionDeltas[i].x,solverImplementation.positionDeltas[i].y,solverImplementation.positionDeltas[i].z});
                if (Global.debugMode)
                {
                    var debug1 = PosRhsCached[i].ToArray();
                    var debug2 = QuatRhsCached[i].ToArray();
                }
                int q = orientationIndices[i];
                //delta与其他约束batch共用内存，所以index不能用i
                var vPosDelta =  Vec.Build.DenseOfArray(
                    new double[]{solverImplementation.positionDeltas[q].x,solverImplementation.positionDeltas[q].y,solverImplementation.positionDeltas[q].z});
                var dPosRhs = PosRhsCached[i] * vPosDelta;
                solverImplementation.CRPosRhs += dPosRhs;
                // var vQuatDelta = Vector.Create<float>(
                //     new float[]{solverImplementation.orientationDeltas[i].value.w,solverImplementation.orientationDeltas[i].value.x,solverImplementation.orientationDeltas[i].value.y,solverImplementation.orientationDeltas[i].value.z});
                var vQuatDelta = Vec.Build.DenseOfArray(
                    new double[]{solverImplementation.orientationDeltas[q].value.w,solverImplementation.orientationDeltas[q].value.x,solverImplementation.orientationDeltas[q].value.y,solverImplementation.orientationDeltas[q].value.z});
                var dQuatRhs = QuatRhsCached[i] * vQuatDelta;
                solverImplementation.CRQuatRhs += dQuatRhs;
            }
            UnityEngine.Profiling.Profiler.EndSample();
            var parameters = solverAbstraction.GetConstraintParameters(m_ConstraintType);
            
            var applyConstraints = new ApplyStretchShearConstraintsBatchJob()
            {
                particleIndices = particleIndices,
                orientationIndices = orientationIndices,
            
                positions = solverImplementation.positions,
                deltas = solverImplementation.positionDeltas,
                counts = solverImplementation.positionConstraintCounts,
            
                orientations = solverImplementation.orientations,
                orientationDeltas = solverImplementation.orientationDeltas,
                orientationCounts = solverImplementation.orientationConstraintCounts,
            
                sorFactor = parameters.SORFactor
            };
            return applyConstraints.Schedule(m_ConstraintCount, 64, inputDeps);
        }

        [BurstCompile]
        public struct StretchShearConstraintsBatchJob : IJobParallelFor
        {
            [ReadOnly] public NativeArray<int> particleIndices;
            [ReadOnly] public NativeArray<int> orientationIndices;
            [ReadOnly] public NativeArray<float> restLengths;
            [ReadOnly] public NativeArray<quaternion> restOrientations;
            //[ReadOnly] [ItemCanBeNull] public List<SparseMatrix> RhsPosMatrixCached;
            //[ReadOnly] [ItemCanBeNull] public List<SparseMatrix> RhsQuatMatrixCached;

            [ReadOnly] public NativeArray<float4> positions;
            [ReadOnly] public NativeArray<quaternion> orientations;
            [ReadOnly] public NativeArray<float> invMasses;
            [ReadOnly] public NativeArray<float> invRotationalMasses;

            [NativeDisableContainerSafetyRestriction][NativeDisableParallelForRestriction] public NativeArray<float4> PosProject;
            [NativeDisableContainerSafetyRestriction][NativeDisableParallelForRestriction] public NativeArray<quaternion> QuatProject;
            
            

            [ReadOnly] public float deltaTimeSqr;

            public void Execute(int i)
            {
                int p1 = particleIndices[i * 2];
                int p2 = particleIndices[i * 2 + 1];
                int q = orientationIndices[i];
                
                // calculate time adjusted compliance
                float3 delta = (positions[p2] - positions[p1]).xyz;

                float3 e = math.rotate(restOrientations[i],new float3(0, 0, 1));
                quaternion basis = math.mul(orientations[q],restOrientations[i]);

                float3 d3 = math.rotate(basis, new float3(0, 0, 1));
               

                var diff_un = Quaternion.FromToRotation(d3, delta);
                var un_star = diff_un * basis;

                PosProject[q] = new float4(d3,0);
                QuatProject[q] = un_star;
                
            }
        }

        [BurstCompile]
        public struct ApplyStretchShearConstraintsBatchJob : IJobParallelFor
        {
            [ReadOnly] public NativeArray<int> particleIndices;
            [ReadOnly] public NativeArray<int> orientationIndices;
            [ReadOnly] public float sorFactor;

            [NativeDisableContainerSafetyRestriction] [NativeDisableParallelForRestriction] public NativeArray<float4> positions;
            [NativeDisableContainerSafetyRestriction] [NativeDisableParallelForRestriction] public NativeArray<float4> deltas;
            [NativeDisableContainerSafetyRestriction] [NativeDisableParallelForRestriction] public NativeArray<int> counts;

            [NativeDisableContainerSafetyRestriction] [NativeDisableParallelForRestriction] public NativeArray<quaternion> orientations;
            [NativeDisableContainerSafetyRestriction] [NativeDisableParallelForRestriction] public NativeArray<quaternion> orientationDeltas;
            [NativeDisableContainerSafetyRestriction] [NativeDisableParallelForRestriction] public NativeArray<int> orientationCounts;

            public void Execute(int i)
            {
                int p1 = particleIndices[i * 2];
                int p2 = particleIndices[i * 2 + 1];
                int q1 = orientationIndices[i];

                deltas[p1] = float4.zero;
                deltas[p2] = float4.zero;
                orientationDeltas[q1] = new quaternion(0, 0, 0, 0);
            }
        }
    }
}
#endif