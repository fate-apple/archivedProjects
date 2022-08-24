#pragma once

#include "CosseratEdges.h"
#include "Chaos/KinematicGeometryParticles.h"
#include "Chaos/PerParticleGravity.h"
#include "Chaos/VelocityField.h"
#include "Chaos/PBDParticles.h"
#include "Chaos/PBDActiveView.h"
#include "Chaos/Vector.h"

namespace Chaos
{
	class FRopeSimulationRope;

	template<class T, int d>
	class PD_API TPDEvolution : public TArrayCollection
{
	public:
	using FVelocityField = TVelocityField<T, d>;
	using FGravityForces = TPerParticleGravity<T, d>;
	TPDEvolution(TPBDParticles<T, d>&& InParticles, TCosseratEdges<T,d>&& InEdges, int32 NumIterations ,T Damping = 0.04);
	int32 AddParticleRange(int32 NumParticles, uint32 GroupId, bool bActivate);
	int32 AddEdgeRange(int32 NumParticles, uint32 GroupId, bool bActivate);
	int32 AddConstraintInitRange(int32 NumConstraints, bool bActivate);
	int32 AddConstraintRuleRange(int32 NumConstraints, bool bActivate);
	~TPDEvolution() {}

	TPBDParticles<T, d>& Particles() { return MParticles; }
	TCosseratEdges<T, d>& Edges() { return MEdges; }

	void AdvanceOneTimeStep(const T dt);
	void AddGroups(int32 NumGroups);
	template<bool bForceRule, bool bVelocityField, bool bDampVelocityRule>
		void PreIterationUpdate(const T Dt, const int32 Offset, const int32 Range, const int32 MinParallelBatchSize);
	void PreIterationQuatUpdate(T Dt, int32 Offset, int32 Range, int32 MinParallelBatchSize);
	void SetAnimationPos(TArray<TVector<int32,1> >& MConstraints,TArray<Vec3>& AnimationPos,
	const int32 Offset,
	const int32 Range,
	const int32 MinParallelBatchSize);

	void ActivateConstraintInitRange(int32 Offset, bool bActivate) { MConstraintInitsActiveView.ActivateRange(Offset, bActivate); }
	void ActivateConstraintRuleRange(int32 Offset, bool bActivate) { MConstraintRulesActiveView.ActivateRange(Offset, bActivate); }
	void ActivateParticleRange(int32 Offset, bool bActivate)  { MParticlesActiveView.ActivateRange(Offset, bActivate); }
	void ActivateEdgeRange(int32 Offset, bool bActivate)  { MEdgesActiveView.ActivateRange(Offset, bActivate); }

	T GetTime() const { return MTime; }
	PDScalar& GetHardConstraintWeight() { return HardConstraintWeight; }
	void SetHardConstraintWeight(const PDScalar Weight) {  HardConstraintWeight = Weight; }
	void SetIterations(const int32 Iterations) { MNumIterations = Iterations; }
	int32 GetParticleRangeSize(int32 Offset) const { return MParticlesActiveView.GetRangeSize(Offset); }
	int32 GetEdgeRangeSize(int32 Offset) const { return MEdgesActiveView.GetRangeSize(Offset); }

	const TArray<TFunction<void(Vec& NewPos,Vec& NewQuat,SparseMatrix&CRPosLhs ,SparseMatrix&CRQuatLhs,Vec&CRPosRhs,Vec&CRQuatRhs,bool bInitLhs )>>& ConstraintRules() const { return MConstraintRules; }
	TArray<TFunction<void(Vec& NewPos,Vec& NewQuat,SparseMatrix&CRPosLhs ,SparseMatrix&CRQuatLhs,Vec&CRPosRhs,Vec&CRQuatRhs,bool bInitLhs )>>& ConstraintRules() { return MConstraintRules; }

	void UpdateMatrix();
	void SetKinematicUpdateFunction(TFunction<void(TPBDParticles<T, d>&, const T, const T, const int32)> KinematicUpdate) { MKinematicUpdate = KinematicUpdate; }
	
	
	private:
	TPBDParticles<T, d> MParticles;
	TPBDActiveView<TPBDParticles<T, d>> MParticlesActiveView;
	TCosseratEdges<T,d> MEdges;
	TPBDActiveView<TCosseratEdges<T,d>> MEdgesActiveView;
	
	TArrayCollectionArray<uint32> MParticleGroupIds;
	TArrayCollectionArray<FVelocityField> MGroupVelocityFields;
	TArrayCollectionArray<TFunction<void(TPBDParticles<T, d>&, const T, const int32)>> MGroupForceRules;
	TArrayCollectionArray<FGravityForces> MGroupGravityForces;

	TArray<TFunction<void(const TPBDParticles<T, d>&, const T)>> MConstraintInits;
	TPBDActiveView<TArray<TFunction<void(const TPBDParticles<T, d>&, const T)>>> MConstraintInitsActiveView;
	TArray<TFunction<void(Vec& NewPos,Vec& NewQuat,SparseMatrix&CRPosLhs ,SparseMatrix&CRQuatLhs,Vec&CRPosRhs,Vec&CRQuatRhs,bool bInitLhs )>> MConstraintRules;
	TPBDActiveView<TArray<TFunction<void(Vec& NewPos,Vec& NewQuat,SparseMatrix&CRPosLhs ,SparseMatrix&CRQuatLhs,Vec&CRPosRhs,Vec&CRQuatRhs,bool bInitLhs )>>> MConstraintRulesActiveView;

	
	//TArray<Vec3>MPosProjections;
	//TArray<Vec4>MQuatProjections;
	SparseMatrix M;			//only change when add or remove particles
	SparseMatrix J;			// <0,j1,j2,j3>

	SparseMatrix CRPosLhs;
	SparseMatrix CRQuatLhs;
	Vec CRPosRhs;
	Vec CRQuatRhs;
	//Vec FlatPos;
	//Vec FlatQuat;
	Vec Sx;	//Flat Pos Vector
	Vec Su;	//Flat Quat Vector
	//Vec CRPosRhs;
	typedef Eigen::SimplicialLDLT<SparseMatrix> SparseSolver;
	SparseSolver CRPosSolver;
	SparseSolver CRQuatSolver;
	PDScalar HardConstraintWeight;

	TFunction<void(TPBDParticles<T, d>&, const T, const T, const int32)> MKinematicUpdate;
	
	int32 MNumIterations;
	TVector<T, d> MGravity;
	T MDamping;
	T MTime;
};
}
