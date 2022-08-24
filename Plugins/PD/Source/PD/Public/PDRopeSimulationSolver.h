#pragma once
#include "CRProperty.h"
#include "PDEvolution.h"


namespace Chaos
{
	class FRopeSimulationRope;
	class FRopeConstraints;
	
	namespace ChaosRopeSimulationSolverDefault
	{
		static const TVector<float, 3> Gravity(0.f, 0.f, -980.665f);  // cm/s^2
		static const TVector<float, 3> WindVelocity(0.f);
		static const float WindFluidDensity = 1.225e-6f;  // kg/cm^3
		static const int32 NumIterations = 4;
		static const int32 NumSubsteps = 4;
		static const float SelfCollisionThickness = 2.f;
		static const float CollisionThickness = 1.2f;
		static const float FrictionCoefficient = 0.2f;
		static const float DampingCoefficient = 0.01f;
	}

	namespace ChaosRopeSimulationSolverConstant
	{
		static const float WorldScale = 0.01f;  // World is in cm, but values like wind speed and density are in SI unit and relates to m.
		static const float StartDeltaTime = 1.f / 30.f;  // Initialize filtered timestep at 30fps
	}

	
	
	class FRopeSimulationSolver final
	{
	public:
		FRopeSimulationSolver();
		~FRopeSimulationSolver();
		void AddRope(FRopeSimulationRope* InRope);
		int32 AddParticles(int32 NumParticles, uint32 GroupId);
		int32 AddEdges(int32 NumParticles, uint32 GroupId);

		void SetNumIterations(int32 InNumIterations) { NumIterations = InNumIterations; }
		int32 GetNumIterations() const { return NumIterations; }
		void SetNumSubsteps(int32 InNumSubsteps) { NumSubsteps = InNumSubsteps; }
		int32 GetNumSubsteps() const { return NumSubsteps; }
		void Update(float InDeltaTime);
		const TVector<float, 3>* GetParticlePs(int32 Offset) const;
		TVector<float, 3>* GetParticlePs(int32 Offset);
		const TVector<float, 3>* GetParticleXs(int32 Offset) const;
		TVector<float, 3>* GetParticleXs(int32 Offset);
		const Quat* GetEdgeQs(int32 Offset) const;
		const TVector<float, 3>* GetParticleVs(int32 Offset) const;
		TVector<float, 3>* GetParticleVs(int32 Offset);
		const float* GetParticleInvMasses(int32 Offset) const;
		void SetParticleMassUniform(int32 Offset, const CRProperty& Property);
		void InitEdges(int32 Offset, const CRProperty& Property);
		void SetEdgeOrientation(int32 PosOffset, int32 QuatOffset,CRProperty Property);
		void UpdateStatus();
		void EnableParticles(int32 Offset, bool bEnable);
		void EnableEdges(int32 Offset, bool bEnable);
		void SetAnimationPos(int32 Offset, TConstArrayView<TVector<float, 3>> InAnimationPos);
		void SetParticleM(int32 Offset, PDScalar InM);

		const TVector<float, 3>* GetOldAnimationPositions(int32 Offset) const { return OldAnimationPositions.GetData() + Offset; }
		TVector<float, 3>* GetOldAnimationPositions(int32 Offset) { return OldAnimationPositions.GetData() + Offset; }
		const TVector<float, 3>* GetAnimationPositions(int32 Offset) const { return AnimationPositions.GetData() + Offset; }
		TVector<float, 3>* GetAnimationPositions(int32 Offset) { return AnimationPositions.GetData() + Offset; }
		FRopeConstraints& GetRopeConstraints(int32 Offset) { return *RopesConstraints.FindChecked(Offset); }
		PDScalar& GetHardConstraintWeight() { return Evolution->GetHardConstraintWeight(); }
		void SetHardConstraintWeight(PDScalar Weight) {  Evolution->SetHardConstraintWeight(Weight); } 

	private:
		TUniquePtr<TPDEvolution<float, 3>> Evolution;

		// Particle attributes
		TArrayCollectionArray<TVector<float, 3>> OldAnimationPositions;
		TArrayCollectionArray<TVector<float, 3>> AnimationPositions;

		// Cloth constraints
		TMap<int32, TUniquePtr<FRopeConstraints>> RopesConstraints;				//Use PosOffset as key

		// Time stepping
		float Time;
		float DeltaTime;
		int32 NumIterations;
		int32 NumSubsteps;

		// Solver parameters
		TVector<float, 3> Gravity;
		TVector<float, 3> WindVelocity;

		// Object arrays
		TArray<FRopeSimulationRope*> Ropes;
	};
}
