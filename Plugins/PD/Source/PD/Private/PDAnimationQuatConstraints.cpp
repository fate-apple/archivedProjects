#include "PDAnimationQuatConstraints.h"

#include "PDAnimationConstraints.h"


using namespace Chaos;

void FPDAnimationQuatConstraints::computeProjections
	(Vec& NewPos,Vec& NewQuat,SparseMatrix&CRPosLhs ,SparseMatrix&CRQuatLhs,Vec&CRPosRhs,Vec&CRQuatRhs,bool bInitLhs )
{

	//PosSelectionMatrix =  SparseMatrix(3, NewPos.rows());
	//PosSelectionMatrix.setZero();
	QuatSelectionMatrix =  SparseMatrix(4, NewQuat.rows());
	QuatSelectionMatrix.setZero();
	
	if(bInitLhs)
	{
		if  (MConstraints.Num() > Chaos_Animation_ParallelConstraintCount)
		{
			PhysicsParallelFor(MConstraints.Num(), [&](const int32 Index) {
				computeProjection( NewPos,NewQuat,Index ,CRPosLhs,CRQuatLhs,CRPosRhs,CRQuatRhs);
			});
		}
		else
		{
			for (int32 i = 0; i < MConstraints.Num(); ++i)
			{
				computeProjection( NewPos,NewQuat,i ,CRPosLhs,CRQuatLhs,CRPosRhs,CRQuatRhs);
			}
		}
	}
	else
	{
		if  (MConstraints.Num() > Chaos_Animation_ParallelConstraintCount)
		{
			PhysicsParallelFor(MConstraints.Num(), [&](const int32 Index) {
				computeProjection( NewPos,NewQuat,Index ,CRPosRhs,CRQuatRhs);
			});
		}
		else
		{
			for (int32 i = 0; i < MConstraints.Num(); ++i)
			{
				computeProjection( NewPos,NewQuat,i ,CRPosRhs,CRQuatRhs);
			}
		}
	}
	return;
}

void FPDAnimationQuatConstraints::computeProjection(Vec& NewPos,Vec& NewQuat,
	const int InConstraintIndex,SparseMatrix&CRPosLhs ,SparseMatrix&CRQuatLhs,Vec&CRPosRhs,Vec&CRQuatRhs)
{
	const int32 i = InConstraintIndex;
	{
		const auto& Constraint = MConstraints[i];
		const int32 e1 = Constraint[0];
		Vec4 target =  Vec4( MAnimationQuat[i].w(),MAnimationQuat[i].x(),MAnimationQuat[i].y(),MAnimationQuat[i].z());
	
		
		//should we cache SelectionMatrix to get better performance?
		QuatSelectionMatrix.setZero();
		//keep in mind, end -start! this implement in A
	
		QuatSelectionMatrix.coeffRef(0,4*e1) =1;
		QuatSelectionMatrix.coeffRef(1,4*e1+1) =1;
		QuatSelectionMatrix.coeffRef(2,4*e1+2) =1;
		QuatSelectionMatrix.coeffRef(3,4*e1+3) =1;
		

		CRQuatRhs+=WeightCoef*QuatSelectionMatrix.transpose()*QuatA.transpose()*target;
		CRQuatLhs+=WeightCoef*QuatSelectionMatrix.transpose()*QuatA.transpose()*QuatA*QuatSelectionMatrix;
	}
}

void FPDAnimationQuatConstraints::computeProjection(Vec& NewPos,Vec& NewQuat,const int InConstraintIndex,Vec&CRPosRhs,Vec&CRQuatRhs)
{
	const int32 i = InConstraintIndex;
	{
		const auto& Constraint = MConstraints[i];
		const int32 e1 = Constraint[0];
		Vec4 target =  Vec4( MAnimationQuat[i].w(),MAnimationQuat[i].x(),MAnimationQuat[i].y(),MAnimationQuat[i].z());
	
		
		//should we cache SelectionMatrix to get better performance?
		QuatSelectionMatrix.setZero();
		//keep in mind, end -start! this implement in A
	
		QuatSelectionMatrix.coeffRef(0,4*e1) =1;
		QuatSelectionMatrix.coeffRef(1,4*e1+1) =1;
		QuatSelectionMatrix.coeffRef(2,4*e1+2) =1;
		QuatSelectionMatrix.coeffRef(3,4*e1+3) =1;
		

		CRQuatRhs+=WeightCoef*QuatSelectionMatrix.transpose()*QuatA.transpose()*target;
	}
}