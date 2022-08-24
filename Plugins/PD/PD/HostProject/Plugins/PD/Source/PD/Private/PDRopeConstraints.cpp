#include "PDRopeConstraints.h"

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


	TFunction<void(const TPBDParticles<PDScalar, 3>&,const TCosseratEdges<PDScalar, 3>&, const PDScalar,SparseMatrix& ,SparseMatrix&,Vec&,Vec&)>* const ConstraintRules = Evolution->ConstraintRules().GetData() + ConstraintRuleOffset;

	int32 ConstraintInitIndex = 0;
	int32 ConstraintRuleIndex = 0;

	if(StretchShearConstraints)
	{
		ConstraintRules[ConstraintRuleIndex++] =
		[this](const TPBDParticles<PDScalar, 3>& InParticles,const TCosseratEdges<PDScalar, 3>& InEdges,
const PDScalar Dt,SparseMatrix& CRPosLhs ,SparseMatrix& CRQuatLhs,Vec& CRPosRhs,Vec& CRQuatRhs)
		{
			StretchShearConstraints->computeProjections(InParticles, InEdges,Dt,CRPosLhs,CRQuatLhs,CRPosRhs,CRQuatRhs);
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