#pragma once
#include "PDRopeConfig.h"
#include "PDTypes.h"
#include "RopeSimulationInterface.h"
#include "Templates/Atomic.h"
#include "Templates/UniquePtr.h"
namespace Chaos
{
	class FRopeSimulationSolver;
	class FRopeSimulationRope;

	class PD_API FRopeSimulation : public IRopeSimulation
{
	public:
	FRopeSimulation();
	virtual ~FRopeSimulation() override;
	//haven't detach rope mesh,physical information for now
	void Simulate(const UPDRopeComponent* context);
	virtual void Initialize() override;
	virtual void CreateActor(UPDRopeComponent* InOwnerComponent, int32 InSimDataIndex) override;
	void GetSimulationData(TMap<int32, FRopeSimulData>& OutData);
	void SetAnimationPos(int Index, TConstArrayView<TVector<float, 3>>& InAnimationPos);
	void SetParticleM(int Index, int Offset, PDScalar InM);

protected:
	//friend class UPDRopeComponent;
	friend class FParallelRopeTask;

	void UpdateSimulationFromSharedSimConfig();
	void UpdateStats(const FRopeSimulationRope* Cloth);
public :

	private:
	void ResetStats();
	private:
	TUniquePtr<FRopeSimulationSolver> Solver;
	TArray<TUniquePtr<FRopeSimulationRope>> Ropes;

	TAtomic<int32> NumRopes;
	TAtomic<int32> NumKinematicParticles;
	TAtomic<int32> NumDynamicParticles;
	TAtomic<int32> NumIterations;
	TAtomic<int32> NumSubsteps;
	TAtomic<float> SimulationTime;
	//TAtomic<bool> bIsTeleported;

	//shared rope config
	UPDRopeSharedSimConfig* RopeSharedSimConfig;
};
}