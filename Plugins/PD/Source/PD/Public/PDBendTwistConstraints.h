#pragma once
#include "CRProperty.h"

namespace Chaos
{
	class FPDBendTwistConstraints
	{
	public:
		FPDBendTwistConstraints( TArray<TVector<int32, 2>>&& Constraints ,const CRProperty&  Property,float Weight)
			:MConstraints(MoveTemp(Constraints)),e3(Property.e3),RopeLength(Property.l0)
		{
			check(MConstraints.Num()!=0);
			SegLength = Property.l0/MConstraints.Num();
			PDScalar J3 = PI*powf(Property.r0,4)/2;
			WeightCoef =  4*Property.G*J3/SegLength;
			MContribution =  4*Property.G*J3/SegLength*MConstraints.Num();
			ComputeLHS();
		}

		PDScalar GetContribution()
		{
			return MContribution;
		}
		
		void ComputeLHS()  {
			QuatA.resize(8,8);
			QuatA.setIdentity();
		}
		
		void computeProjections
	(Vec& NewPos,Vec& NewQuat,SparseMatrix&CRPosLhs ,SparseMatrix&CRQuatLhs,Vec&CRPosRhs,Vec&CRQuatRhs,bool bInitLhs );
		
		void computeProjection(Vec& NewPos,Vec& NewQuat,const int InConstraintIndex,Vec&CRPosRhs,Vec&CRQuatRhs);
		void computeProjection(Vec& NewPos,Vec& NewQuat, const int InConstraintIndex,SparseMatrix&CRPosLhs ,SparseMatrix&CRQuatLhs,Vec&CRPosRhs,Vec&CRQuatRhs);
	protected:
		//should init and keep same.if Lod,create different Constraints 
		Vec3 e3;
		TArray<TVector<int32, 2>> MConstraints;
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
