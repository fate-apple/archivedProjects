#include "PDRopeConstraints.h"

#include "PDBendTwistConstraints.h"
#include "PDEvolution.h"
#include "PDStretchShearConstraints.h"

using namespace Chaos;

FRopeConstraints::FRopeConstraints()
	: Evolution(nullptr)
	, AnimationPositions(nullptr)
	, OldAnimationPositions(nullptr)
	, ParticleOffset(0)
	, NumParticles(0)
	, ConstraintInitOffset(INDEX_NONE)
	, ConstraintRuleOffset(INDEX_NONE)
	, NumConstraintInits(0)
	, NumConstraintRules(0)
{
}

FRopeConstraints::~FRopeConstraints()
{
}

void FRopeConstraints::Initialize(
	TPDEvolution<float, 3>* InEvolution,
	const TArray<TVector<float, 3>>& InAnimationPositions,
	const TArray<TVector<float, 3>>& InOldAnimationPositions,
	int32 InParticleOffset,
	int32 InNumParticles)
{
	Evolution = InEvolution;
	AnimationPositions = &InAnimationPositions;
	OldAnimationPositions = &InOldAnimationPositions;
	ParticleOffset = InParticleOffset;
	NumParticles = InNumParticles;
}

void FRopeConstraints::CreateRules()
{
	check(Evolution);
	check(ConstraintInitOffset == INDEX_NONE)
	if (NumConstraintInits)
	{
		ConstraintInitOffset = Evolution->AddConstraintInitRange(NumConstraintInits, false);
	}
	check(ConstraintRuleOffset == INDEX_NONE)
	if (NumConstraintRules)
	{
		ConstraintRuleOffset = Evolution->AddConstraintRuleRange(NumConstraintRules, false);
	}


	TFunction<void(Vec& NewPos,Vec& NewQuat,SparseMatrix&CRPosLhs ,SparseMatrix&CRQuatLhs,Vec&CRPosRhs,Vec&CRQuatRhs,bool bInitLhs )>* const ConstraintRules = Evolution->ConstraintRules().GetData() + ConstraintRuleOffset;

	int32 ConstraintInitIndex = 0;
	int32 ConstraintRuleIndex = 0;

	if(StretchShearConstraints)
	{
		ConstraintRules[ConstraintRuleIndex++] =
		[this](Vec& NewPos,Vec& NewQuat,SparseMatrix&CRPosLhs ,SparseMatrix&CRQuatLhs,Vec&CRPosRhs,Vec&CRQuatRhs,bool bInitLhs )
		{
			StretchShearConstraints->computeProjections(NewPos, NewQuat,CRPosLhs,CRQuatLhs,CRPosRhs,CRQuatRhs,bInitLhs);
		};
	}
	if(BendTwistConstraints)
	{
		ConstraintRules[ConstraintRuleIndex++] =
		[this](Vec& NewPos,Vec& NewQuat,SparseMatrix&CRPosLhs ,SparseMatrix&CRQuatLhs,Vec&CRPosRhs,Vec&CRQuatRhs,bool bInitLhs )
		{
			BendTwistConstraints->computeProjections(NewPos, NewQuat,CRPosLhs,CRQuatLhs,CRPosRhs,CRQuatRhs,bInitLhs);
		};
	}
	check(ConstraintInitIndex == NumConstraintInits);
	check(ConstraintRuleIndex == NumConstraintRules);
}
void FRopeConstraints::Enable(bool bEnable)
{
	check(Evolution);
	if (ConstraintInitOffset != INDEX_NONE)
	{
		Evolution->ActivateConstraintInitRange(ConstraintInitOffset, bEnable);
	}
	if (ConstraintRuleOffset != INDEX_NONE)
	{
		Evolution->ActivateConstraintRuleRange(ConstraintRuleOffset, bEnable);
	}
}
void FRopeConstraints::SetStretchShearConstraints(TArray<TVector<int32, 3>>& ConstraintsPairs, const CRProperty& Property,float Stiffness)
{
	check(Evolution);
	
	StretchShearConstraints = MakeShared<FPDStretchShearConstraints>(MoveTemp(ConstraintsPairs), Property, Stiffness);

	++NumConstraintRules;
}

void FRopeConstraints::SetBendTwistConstraints(TArray<TVector<int32, 2>>& ConstraintsPairs, const CRProperty& Property,float Stiffness)
{
	check(Evolution);
	
	BendTwistConstraints = MakeShared<FPDBendTwistConstraints>(MoveTemp(ConstraintsPairs), Property, Stiffness);

	++NumConstraintRules;
}
PDScalar FRopeConstraints::GetStretchShearConstraintsContri()
{
	return StretchShearConstraints->GetContribution();
}

PDScalar FRopeConstraints::GetBendTwistConstraintsContri()
{
	return BendTwistConstraints->GetContribution();
}