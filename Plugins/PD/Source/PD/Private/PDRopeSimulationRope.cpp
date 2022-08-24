#include "PDRopeSimulationRope.h"

#include "PDRopeComponent.h"
#include "PDRopeConstraints.h"
#include "PDRopeSimulationSolver.h"

using namespace Chaos;

FRopeSimulationRope::FLODData::FLODData(int32 InNumParticles)
: NumParticles(InNumParticles)
{
}


void FRopeSimulationRope::FLODData::Add(FRopeSimulationSolver* Solver, FRopeSimulationRope* Rope, int32 InLODIndex)
{
	check(Solver);
	check(Rope);
	if (!NumParticles)
	{
		return;
	}

	// Add a new solver data chunk
	check(!SolverData.Find(Solver));
	FSolverData& SolverDatum = SolverData.Add(Solver);
	int32& PosOffset = SolverDatum.PosOffset;
	PosOffset = Solver->AddParticles(NumParticles, Rope->GroupId);
	int32& QuatOffset = SolverDatum.QuatOffset;
	QuatOffset = Solver->AddEdges(NumParticles-1, Rope->GroupId);
	
	Rope->Mesh->Update(Solver, INDEX_NONE, InLODIndex, 0, PosOffset);
	ResetStartPose(Solver);

	
	Solver->SetParticleMassUniform(PosOffset,Rope->Property);
	Solver->InitEdges(QuatOffset,Rope->Property);
	Solver->SetEdgeOrientation(PosOffset,QuatOffset,Rope->Property);
	//Create Rules
	FRopeConstraints& RopeConstraints = Solver->GetRopeConstraints(PosOffset);
	//PDScalar HardConstraintWeight = Solver->GetRopeConstraints();

	auto NumSegment = NumParticles-1;
	TArray<TVector<int32, 3>> SSConstraintsPairs;
	SSConstraintsPairs.Reserve(NumSegment);
	for (int32 i = 0; i < NumSegment; ++i)
	{
		SSConstraintsPairs.Add(
			{ static_cast<int32>(PosOffset +i),
			static_cast<int32>(PosOffset +i+1),
			static_cast<int32>(QuatOffset+i) });
	}
	RopeConstraints.SetStretchShearConstraints(SSConstraintsPairs,Rope->Property);
	Solver->SetHardConstraintWeight( Solver->GetHardConstraintWeight()+RopeConstraints.GetStretchShearConstraintsContri()*10);
	

	TArray<TVector<int32, 2>> BTConstraintsPairs;
	SSConstraintsPairs.Reserve(NumSegment-1);
	for (int32 i = 0; i < NumSegment-1; ++i)
	{
		BTConstraintsPairs.Add({ static_cast<int32>(QuatOffset +i),
			static_cast<int32>(QuatOffset +i+1) });
	}
	RopeConstraints.SetBendTwistConstraints(BTConstraintsPairs,Rope->Property);
	Solver->SetHardConstraintWeight( Solver->GetHardConstraintWeight()+RopeConstraints.GetBendTwistConstraintsContri()*10);
	
	RopeConstraints.CreateRules();

	Solver->UpdateStatus();
	
}

void FRopeSimulationRope::FLODData::Enable(FRopeSimulationSolver* Solver, bool bEnable) const
{
	check(Solver);
	const int32 Offset = SolverData.FindChecked(Solver).PosOffset;
	check(Offset != INDEX_NONE);
	
	// Enable particles (and related constraints)
	Solver->EnableParticles(Offset, bEnable);
	Solver->EnableEdges(Offset, bEnable);
}

void FRopeSimulationRope::FLODData::ResetStartPose(FRopeSimulationSolver* Solver) const
{
	check(Solver);
	const int32 Offset = SolverData.FindChecked(Solver).PosOffset;
	check(Offset != INDEX_NONE);

	TVector<float, 3>* const Ps = Solver->GetParticlePs(Offset);
	TVector<float, 3>* const Xs = Solver->GetParticleXs(Offset);
	TVector<float, 3>* const Vs = Solver->GetParticleVs(Offset);
	const TVector<float, 3>* const AnimationPositions = Solver->GetAnimationPositions(Offset);
	TVector<float, 3>* const OldAnimationPositions = Solver->GetOldAnimationPositions(Offset);

	for (int32 Index = 0; Index < NumParticles; ++Index)
	{
		Ps[Index] = Xs[Index] = OldAnimationPositions[Index] = AnimationPositions[Index];
		Vs[Index] = TVector<float, 3>(0.f);
	}
}

FRopeSimulationRope::FRopeSimulationRope(
	UPDRopeComponent* InMesh,
	uint32 InGroupId,
	bool bUseLODIndexOverride,
	int32 LODIndexOverride
	)
		:GroupId(InGroupId),
		Property(InMesh->RopeLengthV,InMesh->E,InMesh->PoissonRatio,InMesh->density,InMesh->r0,InMesh->e3,InMesh->EndM), //use m instead cm
bUseLODIndexOverride(bUseLODIndexOverride),
LODIndexOverride(LODIndexOverride)
{
	SetMesh(InMesh);
}

FRopeSimulationRope::~FRopeSimulationRope()
{
}

//TODO: maybe decouple mesh and rope data
void FRopeSimulationRope::SetMesh(UPDRopeComponent* InMesh)
{
	Mesh = InMesh;

	// Reset LODs
	const int32 NumLODs = Mesh ? Mesh->GetNumLODs() : 0;
	LODData.Reset(NumLODs);
	LODData.Emplace(Mesh->GetNumPoints());
	// Iterate all known solvers
	TArray<FRopeSimulationSolver*> Solvers;
	LODIndices.GetKeys(Solvers);
	for (FRopeSimulationSolver* const Solver : Solvers)
	{
		//TODO: Refresh this cloth to recreate particles
		//Solver->RefreshCloth(this);
	}
}

void FRopeSimulationRope::Add(FRopeSimulationSolver* Solver)
{
	check(Solver);

	// Can't add a cloth twice to the same solver
	check(!LODIndices.Find(Solver));
	int32& LODIndex = LODIndices.Add(Solver);
	LODIndex = INDEX_NONE;
	for (int32 Index = 0; Index < LODData.Num(); ++Index)
	{
		LODData[Index].Add(Solver, this, Index);
	}
}

void FRopeSimulationRope::PreUpdate(FRopeSimulationSolver* Solver)
{
	
}
void FRopeSimulationRope::Update(FRopeSimulationSolver* Solver)
{
	check(Solver);

	// Exit if the input mesh is missing
	if (!Mesh)
	{
		return;
	}

	// Retrieve LOD Index, either from the override, or from the mesh input
	int32& LODIndex = LODIndices.FindChecked(Solver);  // Must be added to solver first

	const int32 PrevLODIndex = LODIndex;
	LODIndex = bUseLODIndexOverride && LODData.IsValidIndex(LODIndexOverride) ? LODIndexOverride : Mesh->GetLODIndex();
	if (LODIndex != PrevLODIndex)
	{
		if (PrevLODIndex != INDEX_NONE)
		{
			// Disable previous LOD's particles
			LODData[PrevLODIndex].Enable(Solver, false);
		}
		if (LODIndex != INDEX_NONE)
		{
			// Enable new LOD's particles
			LODData[LODIndex].Enable(Solver, true);
		}
	}
}

int32 FRopeSimulationRope::GetOffset(const FRopeSimulationSolver* Solver, int32 InLODIndex) const
{
	return LODData.IsValidIndex(InLODIndex) ? LODData[InLODIndex].SolverData.FindChecked(Solver).PosOffset : 0;
}
int32 FRopeSimulationRope::GetNumParticles(int32 InLODIndex) const
{
	return LODData.IsValidIndex(InLODIndex) ? LODData[InLODIndex].NumParticles : 0;
}
TConstArrayView<TVector<float, 3>> FRopeSimulationRope::GetParticlePositions(const FRopeSimulationSolver* Solver) const
{
	check(Solver);
	const int32 LODIndex = LODIndices.FindChecked(Solver);
	check(GetOffset(Solver, LODIndex) != INDEX_NONE);
	return TConstArrayView<TVector<float, 3>>(Solver->GetParticleXs(GetOffset(Solver, LODIndex)), GetNumParticles(LODIndex));
}

TConstArrayView<Quat> FRopeSimulationRope::GetEdgeQuats(const FRopeSimulationSolver* Solver) const
{
	check(Solver);
	const int32 LODIndex = LODIndices.FindChecked(Solver);
	check(GetOffset(Solver, LODIndex) != INDEX_NONE);
	TConstArrayView<Quat> result (Solver->GetEdgeQs(GetOffset(Solver, LODIndex)), GetNumParticles(LODIndex)-1);
	return result;
}

void FRopeSimulationRope::SetAnimationPos( FRopeSimulationSolver* Solver,TConstArrayView<TVector<float, 3>> InAnimationPos) 
{
	check(Solver);
	const int32 LODIndex = LODIndices.FindChecked(Solver);
	check(GetOffset(Solver, LODIndex) != INDEX_NONE);
	Solver->SetAnimationPos(GetOffset(Solver, LODIndex),InAnimationPos);

}
void FRopeSimulationRope::SetParticleM( FRopeSimulationSolver* Solver,int Offset,PDScalar InM) 
{
	check(Solver);
	const int32 LODIndex = LODIndices.FindChecked(Solver);
	check(GetOffset(Solver, LODIndex) != INDEX_NONE);
	Solver->SetParticleM(GetOffset(Solver, LODIndex)+Offset,InM);

}