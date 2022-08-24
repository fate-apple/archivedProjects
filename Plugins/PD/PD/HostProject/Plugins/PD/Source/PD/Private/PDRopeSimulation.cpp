#include "PDRopeSimulation.h"
#include "PDRopeComponent.h"
#include "PDRopeSimulationRope.h"
#include "PDRopeSimulationSolver.h"
#include "Programs/UnrealLightmass/Private/ImportExport/3DVisualizer.h"
#include "Programs/UnrealLightmass/Private/ImportExport/3DVisualizer.h"

using namespace Chaos;

FRopeSimulation::FRopeSimulation()
	: RopeSharedSimConfig(nullptr)
{
}

Chaos::FRopeSimulation::~FRopeSimulation()
{}

void FRopeSimulation::Simulate(const UPDRopeComponent* Context)
{
	Solver->Update(Context->DeltaSeconds);
}
void FRopeSimulation::ResetStats()
{
	check(Solver);
	NumRopes = 0;
	NumKinematicParticles = 0;
	NumDynamicParticles = 0;
	SimulationTime = 0.f;
	NumSubsteps = Solver->GetNumSubsteps();
	NumIterations = Solver->GetNumIterations();
}
void FRopeSimulation::Initialize()
{
	// Create solver
	Solver = MakeUnique<FRopeSimulationSolver>();
	ResetStats();
}



void FRopeSimulation::CreateActor(UPDRopeComponent* InOwnerComponent,int32 InSimDataIndex)
{
	check(InOwnerComponent);
	check(Solver);
	if (!RopeSharedSimConfig)
	{
		//RopeSharedSimConfig = InOwnerComponent->GetSharedClothConfig();
		RopeSharedSimConfig = UPDRopeSharedSimConfig::StaticClass()->GetDefaultObject<UPDRopeSharedSimConfig>();
		UpdateSimulationFromSharedSimConfig();
	}
	const int32 RopeIndex = Ropes.Emplace(MakeUnique<FRopeSimulationRope>(
		InOwnerComponent,
		InSimDataIndex,
		false,
		0
		));
	Solver->AddRope(Ropes[RopeIndex].Get());
	UpdateStats(Ropes[RopeIndex].Get());
}

void FRopeSimulation::UpdateSimulationFromSharedSimConfig()
{
	check(Solver);
	if (RopeSharedSimConfig) // ClothSharedSimConfig will be a null pointer if all cloth instances are disabled in which case we will use default Evolution parameters
		{
		
		// Set common simulation parameters
		Solver->SetNumSubsteps(RopeSharedSimConfig->SubdivisionCount);
		Solver->SetNumIterations(RopeSharedSimConfig->IterationCount);
		}
}
void FRopeSimulation::UpdateStats(const FRopeSimulationRope* Cloth)
{
	
}

void FRopeSimulation::GetSimulationData(
	TMap<int32, FRopeSimulData>& OutData)
{
	for (const TUniquePtr<FRopeSimulationRope>& Rope : Ropes)
	{
		const int32 AssetIndex = Rope->GetGroupId();
		FRopeSimulData& Data = OutData.FindOrAdd(AssetIndex);
		
		Data.Positions = Rope->GetParticlePositions(Solver.Get());
		//Data.Normals = Cloth->GetParticleNormals(Solver.Get());
	}
}


//TODO
void FRopeSimulation::SetAnimationPos(int Index,TConstArrayView<TVector<float, 3>>& InAnimationPos  )
{
	TUniquePtr<FRopeSimulationRope>& Rope = Ropes[Index];
	Rope->SetAnimationPos(Solver.Get(),InAnimationPos);
}

void FRopeSimulation::SetParticleM(int Index,int Offset,PDScalar InM  )
{
	TUniquePtr<FRopeSimulationRope>& Rope = Ropes[Index];
	Rope->SetParticleM(Solver.Get(),Offset,InM);
}