#include "PDBendTwistConstraints.h"

#include "PDStretchShearConstraints.h"

using namespace Chaos;

void FPDBendTwistConstraints::computeProjections
	(Vec& NewPos,Vec& NewQuat,SparseMatrix&CRPosLhs ,SparseMatrix&CRQuatLhs,Vec&CRPosRhs,Vec&CRQuatRhs,bool bInitLhs )
{

	//PosSelectionMatrix =  SparseMatrix(3, NewPos.rows());
	//PosSelectionMatrix.setZero();
	QuatSelectionMatrix =  SparseMatrix(8, NewQuat.rows());
	QuatSelectionMatrix.setZero();
	
	if(bInitLhs)
	{
		if  (MConstraints.Num() > Chaos_Spring_ParallelConstraintCount)
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
		if  (MConstraints.Num() > Chaos_Spring_ParallelConstraintCount)
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

void FPDBendTwistConstraints::computeProjection(Vec& NewPos,Vec& NewQuat,
	const int InConstraintIndex,SparseMatrix&CRPosLhs ,SparseMatrix&CRQuatLhs,Vec&CRPosRhs,Vec&CRQuatRhs)
{
	const int32 i = InConstraintIndex;
	{
		const auto& Constraint = MConstraints[i];
		const int32 e1 = Constraint[0];
		const int32 e2 = Constraint[1];
	
		
		//should we cache SelectionMatrix to get better performance?
		QuatSelectionMatrix.setZero();
		//keep in mind, end -start! this implement in A
	
		QuatSelectionMatrix.coeffRef(0,4*e1) =1;
		QuatSelectionMatrix.coeffRef(1,4*e1+1) =1;
		QuatSelectionMatrix.coeffRef(2,4*e1+2) =1;
		QuatSelectionMatrix.coeffRef(3,4*e1+3) =1;
		QuatSelectionMatrix.coeffRef(4,4*e2) =1;
		QuatSelectionMatrix.coeffRef(5,4*e2+1) =1;
		QuatSelectionMatrix.coeffRef(6,4*e2+2) =1;
		QuatSelectionMatrix.coeffRef(7,4*e2+3) =1;
		
		Quat u1 = Quat(NewQuat[4*e1],NewQuat[4*e1+1],NewQuat[4*e1+2],NewQuat[4*e1+3]).normalized();
		Quat u2 = Quat(NewQuat[4*e2],NewQuat[4*e2+1],NewQuat[4*e2+2],NewQuat[4*e2+3]).normalized();
		// Quat Curv1 = u1.conjugate()*u2;
		// auto RMatrix = Curv1.normalized().toRotationMatrix();
		// Curv1 = Quat(RMatrix/2);
		//
		// Quat u1_star = u1*Curv1;
		// Quat u2_star = u2*Curv1.conjugate();
		Quat u1_star = u1.slerp(0.5,u2);
		Quat u2_star = u1_star;

		Vec8 target;
		target<<u1_star.w(),u1_star.x(),u1_star.y(),u1_star.z(),u2_star.w(),u2_star.x(),u2_star.y(),u2_star.z();

		CRQuatRhs+=WeightCoef*QuatSelectionMatrix.transpose()*QuatA.transpose()*target;
		CRQuatLhs+=WeightCoef*QuatSelectionMatrix.transpose()*QuatA.transpose()*QuatA*QuatSelectionMatrix;

		//auto debugCRQuatLhs = EigenMatrix2StdVector(CRQuatLhs);
		//auto debugCRQuatRhs = EigenMatrix2StdVector(WeightCoef*QuatSelectionMatrix.transpose()*QuatA.transpose()*target);
		
	}
}

void FPDBendTwistConstraints::computeProjection(Vec& NewPos,Vec& NewQuat,const int InConstraintIndex,Vec&CRPosRhs,Vec&CRQuatRhs)
{
	const int32 i = InConstraintIndex;
	{
		const auto& Constraint = MConstraints[i];
		const int32 e1 = Constraint[0];
		const int32 e2 = Constraint[1];
	
		
		//should we cache SelectionMatrix to get better performance?
		QuatSelectionMatrix.setZero();
		//keep in mind, end -start! this implement in A
	
		QuatSelectionMatrix.coeffRef(0,4*e1) =1;
		QuatSelectionMatrix.coeffRef(1,4*e1+1) =1;
		QuatSelectionMatrix.coeffRef(2,4*e1+2) =1;
		QuatSelectionMatrix.coeffRef(3,4*e1+3) =1;
		QuatSelectionMatrix.coeffRef(4,4*e2) =1;
		QuatSelectionMatrix.coeffRef(5,4*e2+1) =1;
		QuatSelectionMatrix.coeffRef(6,4*e2+2) =1;
		QuatSelectionMatrix.coeffRef(7,4*e2+3) =1;
		
		Quat u1 = Quat(NewQuat[4*e1],NewQuat[4*e1+1],NewQuat[4*e1+2],NewQuat[4*e1+3]);
		Quat u2 = Quat(NewQuat[4*e2],NewQuat[4*e2+1],NewQuat[4*e2+2],NewQuat[4*e2+3]);
		// Quat Curv1 = u1.conjugate()*u2;
		// auto RMatrix = Curv1.normalized().toRotationMatrix();
		// Curv1 = Quat(RMatrix/2);
		//
		// Quat u1_star = u1*Curv1;
		// Quat u2_star = u2*Curv1.conjugate();
		Quat u1_star = u1.slerp(0.5,u2);
		Quat u2_star = u1_star;

		Vec target;
		target.resize(8);
		target<<u1_star.w(),u1_star.x(),u1_star.y(),u1_star.z(),u2_star.w(),u2_star.x(),u2_star.y(),u2_star.z();

		CRQuatRhs+=WeightCoef*QuatSelectionMatrix.transpose()*QuatA.transpose()*target;
	}
}