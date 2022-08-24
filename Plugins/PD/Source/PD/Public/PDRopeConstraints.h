#pragma once
#include "CRProperty.h"


namespace Chaos
{
	class FPDBendTwistConstraints;
	class FPDStretchShearConstraints;
	template<typename T, int d> class TPDEvolution;
	
	class FRopeConstraints final
	{
public:
		FRopeConstraints();
		~FRopeConstraints();
		void CreateRules();
		void Enable(bool bEnable);
		void Initialize(
			TPDEvolution<float, 3>* InEvolution,
			const TArray<TVector<float, 3>>& InAnimationPositions,
			const TArray<TVector<float, 3>>& InOldAnimationPositions,
			int32 InParticleOffset,
			int32 InNumParticles);
		void SetStretchShearConstraints(TArray<TVector<int32, 3>>& ConstraintsPairs, const CRProperty& Property,float Stiffness=1);
		void SetBendTwistConstraints(TArray<TVector<int32, 2>>& ConstraintsPairs, const CRProperty& Property,float Stiffness=1);
		PDScalar GetStretchShearConstraintsContri();
		PDScalar GetBendTwistConstraintsContri();


	private:
		TSharedPtr<FPDStretchShearConstraints> StretchShearConstraints;
		TSharedPtr<FPDBendTwistConstraints> BendTwistConstraints;
		
		
		TPDEvolution<float, 3>* Evolution;
		const TArray<TVector<float, 3>>* AnimationPositions;
		const TArray<TVector<float, 3>>* OldAnimationPositions;

		int32 ParticleOffset;
		int32 NumParticles;
		int32 ConstraintInitOffset;
		int32 ConstraintRuleOffset;
		int32 NumConstraintInits;
		int32 NumConstraintRules;
		
	};
}
