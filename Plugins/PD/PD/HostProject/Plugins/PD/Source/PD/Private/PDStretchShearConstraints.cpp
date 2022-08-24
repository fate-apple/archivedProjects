#include "PDStretchShearConstraints.h"
#include "CosseratEdges.h"


using namespace Chaos;
void FPDStretchShearConstraints::computeProjections
	(const TPBDParticles<PDScalar, 3>& InParticles,const TCosseratEdges<PDScalar, 3>& InEdges,PDScalar Dt,SparseMatrix&CRPosLhs ,SparseMatrix&CRQuatLhs,Vec&CRPosRhs,Vec&CRQuatRhs)
{
	//PosProj.AddUninitialized(InParticles.Size());
	//QuatProj.AddUninitialized(InEdges.Size());
	PosSelectionMatrix =  SparseMatrix(6, 3*InParticles.Size());
	PosSelectionMatrix.setZero();
	QuatSelectionMatrix =  SparseMatrix(4, 4*InEdges.Size());
	QuatSelectionMatrix.setZero();
	if  (MConstraints.Num() > Chaos_Spring_ParallelConstraintCount)
	{
		PhysicsParallelFor(MConstraints.Num(), [&](const int32 Index) {
			computeProjection(InParticles,InEdges,Dt, Index ,CRPosLhs,CRQuatLhs,CRPosRhs,CRQuatRhs);
		});
	}
	else
	{
		for (int32 i = 0; i < MConstraints.Num(); ++i)
		{
			computeProjection(InParticles,InEdges, Dt, i,CRPosLhs,CRQuatLhs,CRPosRhs,CRQuatRhs);
		}
	}
	return;
}

void FPDStretchShearConstraints::computeProjection(const TPBDParticles<PDScalar, 3>& InParticles,const TCosseratEdges<PDScalar, 3>& InEdges,
	const PDScalar Dt,const int InConstraintIndex,SparseMatrix&CRPosLhs ,SparseMatrix&CRQuatLhs,Vec&CRPosRhs,Vec&CRQuatRhs)
{
	const int32 i = InConstraintIndex;
	{
		const auto& Constraint = MConstraints[i];
		const int32 i1 = Constraint[0];
		const int32 i2 = Constraint[1];
		const int32 e1 = Constraint[2];
		auto P1 = InParticles.P(i1);
		auto P2 = InParticles.P(i2);
		auto Q1 = InEdges.Su(e1);

		Vec3 xf,d3;
		Quat diff_un,un_star;
		Vec4 qi;
		
		TVector<PDScalar,3> Delta = (P2 - P1) / SegLength; //Vec P1P2
		xf<<Delta.X,Delta.Y,Delta.Z;
		d3 = Q1.normalized().toRotationMatrix() * e3;
        
		diff_un = Quat::FromTwoVectors(xf, d3);
		un_star = Q1*diff_un;
		qi<<un_star.w(),un_star.x(),un_star.y(),un_star.z();			//J=<0,j1,j2,j3>
		
		// CRPosRhs[3*i1] += WPosRhs*d3.x();
		// CRPosRhs[3*i1+1] += WPosRhs*d3.y();
		// CRPosRhs[3*i1+2] += WPosRhs*d3.z();
		// CRPosRhs[3*i2] -= WPosRhs*d3.x();
		// CRPosRhs[3*i2+1] -= WPosRhs*d3.y();
		// CRPosRhs[3*i2+2] -= WPosRhs*d3.z();
		//
		// CRQuatRhs[4*e1]+= WQuatLhs*un_star.w();
		// CRQuatRhs[4*e1+1]+= WQuatLhs*un_star.x();
		// CRQuatRhs[4*e1+2]+= WQuatLhs*un_star.y();
		// CRQuatRhs[4*e1+3]+= WQuatLhs*un_star.z();
		PosSelectionMatrix.setZero();
		PosSelectionMatrix.coeffRef(0,3*i2) =1;
		PosSelectionMatrix.coeffRef(1,3*i2+1) =1;
		PosSelectionMatrix.coeffRef(2,3*i2+2) =1;
		PosSelectionMatrix.coeffRef(3,3*i1) =1;
		PosSelectionMatrix.coeffRef(4,3*i1+1) =1;
		PosSelectionMatrix.coeffRef(5,3*i1+2) =1;
		QuatSelectionMatrix.coeffRef(0,4*e1) =1;
		QuatSelectionMatrix.coeffRef(1,4*e1+1) =1;
		QuatSelectionMatrix.coeffRef(2,4*e1+2) =1;
		QuatSelectionMatrix.coeffRef(3,4*e1+3) =1;

		//auto PosTemp = WeightCoef*PosSelectionMatrix.transpose()*PosA.transpose();
		CRPosRhs+=WeightCoef*PosSelectionMatrix.transpose()*PosA.transpose()*d3;
		CRQuatRhs+=WeightCoef*QuatSelectionMatrix.transpose()*QuatA.transpose()*qi;
		CRPosLhs+=WeightCoef*PosSelectionMatrix.transpose()*PosA.transpose()*PosA*PosSelectionMatrix;
		CRQuatLhs +=WeightCoef*QuatSelectionMatrix.transpose()*QuatA.transpose()*QuatA*QuatSelectionMatrix;
	}
}