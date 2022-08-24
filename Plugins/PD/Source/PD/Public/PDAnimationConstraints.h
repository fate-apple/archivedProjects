#pragma once

#include "CRProperty.h"

#include "PDTypes.h"

#include "Chaos/Framework/Parallel.h"
namespace Chaos
{
	static int32 Chaos_Animation_ParallelConstraintCount = 100;
	//template<class T, int d>
	class FPDAnimationConstraints
	{
	public:
		FPDAnimationConstraints( const CRProperty&  Property,float Weight)
			:RopeLength(Property.l0)
		{
	
			WeightCoefInit =  Property.E*Property.A*Property.l0/MConstraints.Num()*Weight;
			ComputeLHS();
		}
		FPDAnimationConstraints( TArray<TVector<int32, 1>>&& Constraints ,TArray<Vec3>&& AnimationPos,const CRProperty&  Property,float Weight)
			:MConstraints(MoveTemp(Constraints)),MAnimationPos(MoveTemp(AnimationPos)),RopeLength(Property.l0)
		{
			check(MConstraints.Num()!=0);
			WeightCoefInit =  Property.E*Property.A*Property.l0/MConstraints.Num()*Weight;
			WeightCoef =  WeightCoefInit/MConstraints.Num();
			ComputeLHS();
		}
		FPDAnimationConstraints( TArray<TVector<int32, 1>>&& Constraints ,TArray<Vec3>&& AnimationPos,float Weight)
			:MConstraints(MoveTemp(Constraints)),MAnimationPos(MoveTemp(AnimationPos)),WeightCoefInit(0),WeightCoef(Weight)
		{
			check(MConstraints.Num()!=0);
			ComputeLHS();
		}
		
		void  SetConstraints(TArray<TVector<int32, 1>>&& Constraints)
		{
			check(MConstraints.Num()!=0);
			MConstraints = MoveTemp(Constraints);
			WeightCoef =  WeightCoefInit/MConstraints.Num();
		}
		void ComputeLHS()  {

			PosA.resize(3, 3);
			PosA.setIdentity();
			
			QuatA.resize(4,4);
			QuatA.setIdentity();
		}
		
		void computeProjections(Vec& NewPos,Vec& NewQuat,SparseMatrix&CRPosLhs ,SparseMatrix&CRQuatLhs,Vec&CRPosRhs,Vec&CRQuatRhs,bool bInitLhs );
		
		void computeProjection(Vec& NewPos,Vec& NewQuat,const int InConstraintIndex,Vec&CRPosRhs,Vec&CRQuatRhs);
		void computeProjection(Vec& NewPos,Vec& NewQuat, const int InConstraintIndex,SparseMatrix&CRPosLhs ,SparseMatrix&CRQuatLhs,Vec&CRPosRhs,Vec&CRQuatRhs);
	protected:
		//should init and keep same.if Lod,create different Constraints 
		Vec3 e3;
		TArray<TVector<int32,1> > MConstraints;
		TArray<Vec3> MAnimationPos;
		PDScalar  WeightCoef;
		PDScalar  WeightCoefInit;
		PDScalar SegLength;
		PDScalar RopeLength;
		SparseMatrix PosA;
		SparseMatrix QuatA;


		SparseMatrix PosSelectionMatrix;		//6, 3*InParticles.Size()
		SparseMatrix QuatSelectionMatrix;		//4, 4*InEdges.size()
	};
	
}