#pragma once
#include "CosseratEdges.h"
#include "CRProperty.h"
#include "Chaos/DynamicParticles.h"
#include "PDTypes.h"
#include "Chaos/PBDParticles.h"
#include "Chaos/Framework/Parallel.h"
namespace Chaos
{
	static int32 Chaos_Spring_ParallelConstraintCount = 100;
	//template<class T, int d>
	class FPDStretchShearConstraints
	{
	public:
		FPDStretchShearConstraints( TArray<TVector<int32, 3>>&& Constraints ,const CRProperty&  Property,float Weight)
			:MConstraints(MoveTemp(Constraints)),e3(Property.e3),RopeLength(Property.l0)
		{
			check(MConstraints.Num()!=0);
			SegLength = Property.l0/MConstraints.Num();
			WeightCoef =  Property.E*Property.A*SegLength*Weight;
			MContribution =  Property.E*Property.A* Property.l0*Weight;
			ComputeLHS();
		}

		PDScalar GetContribution()
		{
			return MContribution;
		}
		
		void ComputeLHS()  {
		
			PosA.resize(3, 6);
			PosA.setZero();
			const float x = 1/SegLength;
			PosA.coeffRef(0,0) = x;
			PosA.coeffRef(1,1) = x;
			PosA.coeffRef(2,2) = x;
			PosA.coeffRef(0,3) = -x;
			PosA.coeffRef(1,4) = -x;
			PosA.coeffRef(2,5) = -x;
			QuatA.resize(4,4);
			QuatA.setIdentity();
		}
		void computeProjections(const TPBDParticles<PDScalar, 3>& InParticles, const TCosseratEdges<PDScalar, 3>& InEdges, float Dt, SparseMatrix& CRPosLhs, SparseMatrix
		                        & CRQuatLhs, Vec& CRPosRhs, Vec& CRQuatRhs);
		void computeProjections
	(Vec& NewPos,Vec& NewQuat,SparseMatrix&CRPosLhs ,SparseMatrix&CRQuatLhs,Vec&CRPosRhs,Vec&CRQuatRhs,bool bInitLhs );
		void computeProjection(const TPBDParticles<PDScalar, 3>& InParticles,const TCosseratEdges<PDScalar, 3>& InEdges,
	const PDScalar Dt,const int InConstraintIndex,SparseMatrix&CRPosLhs ,SparseMatrix&CRQuatLhs,Vec&CRPosRhs,Vec&CRQuatRhs);
		void computeProjection(Vec& NewPos,Vec& NewQuat,const int InConstraintIndex,Vec&CRPosRhs,Vec&CRQuatRhs);
		void computeProjection(Vec& NewPos,Vec& NewQuat, const int InConstraintIndex,SparseMatrix&CRPosLhs ,SparseMatrix&CRQuatLhs,Vec&CRPosRhs,Vec&CRQuatRhs);
	protected:
		//should init and keep same.if Lod,create different Constraints 
		Vec3 e3;
		TArray<TVector<int32, 3>> MConstraints;
		PDScalar  WeightCoef;
		PDScalar SegLength;
		PDScalar RopeLength;
		SparseMatrix PosA;
		SparseMatrix QuatA;
		PDScalar MContribution;

		
		SparseMatrix PosSelectionMatrix;		//6, 3*InParticles.Size()
		SparseMatrix QuatSelectionMatrix;		//4, 4*InEdges.size()
	};
	
}