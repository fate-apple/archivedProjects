#include "PDAnimationConstraints.h"


using namespace Chaos;

void FPDAnimationConstraints::computeProjections
	(Vec& NewPos,Vec& NewQuat,SparseMatrix&CRPosLhs ,SparseMatrix&CRQuatLhs,Vec&CRPosRhs,Vec&CRQuatRhs,bool bInitLhs )
{

	PosSelectionMatrix =  SparseMatrix(3, NewPos.rows());
	PosSelectionMatrix.setZero();
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

void FPDAnimationConstraints::computeProjection(Vec& NewPos,Vec& NewQuat,
	const int InConstraintIndex,SparseMatrix&CRPosLhs ,SparseMatrix&CRQuatLhs,Vec&CRPosRhs,Vec&CRQuatRhs)
{
	const int32 i = InConstraintIndex;
	{
		const auto& Constraint = MConstraints[i];
		const int32 i1 = Constraint[0];
		Vec3 target = MAnimationPos[i];
	
		
		//should we cache SelectionMatrix to get better performance?
		PosSelectionMatrix.setZero();
		//keep in mind, end -start! this implement in A
	
		PosSelectionMatrix.coeffRef(0,3*i1) =1;
		PosSelectionMatrix.coeffRef(1,3*i1+1) =1;
		PosSelectionMatrix.coeffRef(2,3*i1+2) =1;
		

		CRPosRhs+=WeightCoef*PosSelectionMatrix.transpose()*PosA.transpose()*target;
		CRPosLhs+=WeightCoef*PosSelectionMatrix.transpose()*PosA.transpose()*PosA*PosSelectionMatrix;
	}
}

void FPDAnimationConstraints::computeProjection(Vec& NewPos,Vec& NewQuat,const int InConstraintIndex,Vec&CRPosRhs,Vec&CRQuatRhs)
{
	const int32 i = InConstraintIndex;
	{
		const auto& Constraint = MConstraints[i];
		const int32 i1 = Constraint[0];
		Vec3 target = MAnimationPos[i];
	
		
		//should we cache SelectionMatrix to get better performance?
		PosSelectionMatrix.setZero();
		//keep in mind, end -start! this implement in A
	
		PosSelectionMatrix.coeffRef(0,3*i1) =1;
		PosSelectionMatrix.coeffRef(1,3*i1+1) =1;
		PosSelectionMatrix.coeffRef(2,3*i1+2) =1;
		

		CRPosRhs+=WeightCoef*PosSelectionMatrix.transpose()*PosA.transpose()*target;
	}
}