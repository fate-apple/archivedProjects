#include "PDRopeSimulationSolver.h"

#include "PDRopeConstraints.h"
#include "PDRopeSimulationRope.h"

using namespace Chaos;


FRopeSimulationSolver::FRopeSimulationSolver()
: Time(0)
, DeltaTime(ChaosRopeSimulationSolverConstant::StartDeltaTime)
, NumIterations(ChaosRopeSimulationSolverDefault::NumIterations)
, NumSubsteps(ChaosRopeSimulationSolverDefault::NumSubsteps)
, Gravity(ChaosRopeSimulationSolverDefault::Gravity)
, WindVelocity(ChaosRopeSimulationSolverDefault::WindVelocity)
{
	TPBDParticles<float, 3> LocalParticles;
	//TKinematicGeometryClothParticles<float, 3> TRigidParticles;
	TCosseratEdges<float,3> LocalEdges;
	

	Evolution.Reset(
		new TPDEvolution<float, 3>(
			MoveTemp(LocalParticles),
			MoveTemp(LocalEdges),
			ChaosRopeSimulationSolverDefault::NumIterations,
			ChaosRopeSimulationSolverDefault::DampingCoefficient));
	Evolution->Particles().AddArray(&OldAnimationPositions);
	Evolution->Particles().AddArray(&AnimationPositions);
	
	Evolution->SetKinematicUpdateFunction(
    		[this](TPBDParticles<PDScalar, 3>& ParticlesInput, const float Dt, const float LocalTime, const int32 Index)
    		{
    			const float Alpha = (LocalTime - Time) / DeltaTime;
    			ParticlesInput.P(Index) = Alpha * AnimationPositions[Index] + (1.f - Alpha) * OldAnimationPositions[Index];  // X is the step initial condition, here it's P that needs to be updated so that constraints works with the correct step target
    		});

}

FRopeSimulationSolver::~FRopeSimulationSolver()
{
	
}

void FRopeSimulationSolver::AddRope(FRopeSimulationRope* InRope)
{
	check(InRope);

	if (Ropes.Find(InRope) != INDEX_NONE)
	{
		return;
	}
	Ropes.Emplace(InRope);

	InRope->Add(this);

	// Set initial state
	InRope->PreUpdate(this);
	InRope->Update(this);
}

int32 FRopeSimulationSolver::AddParticles(int32 NumParticles, uint32 GroupId)
{
	if (!NumParticles)
	{
		return INDEX_NONE;
	}
	const int32 Offset = Evolution->AddParticleRange(NumParticles, GroupId, /*bActivate =*/ false);

	// Add an empty constraints container for this range
	check(!RopesConstraints.Find(Offset));  // We cannot already have this Offset in the map, particle ranges are always added, never removed (unless reset)
	
	RopesConstraints.Emplace(Offset, MakeUnique<FRopeConstraints>())
		->Initialize(Evolution.Get(), AnimationPositions, OldAnimationPositions, Offset, NumParticles);
	// Always starts with particles disabled
	//EnableParticles(Offset, false);
	return Offset;
}
int32 FRopeSimulationSolver::AddEdges(int32 NumEdges, uint32 GroupId)
{
	if (!NumEdges)
	{
		return INDEX_NONE;
	}
	const int32 Offset = Evolution->AddEdgeRange(NumEdges, GroupId, /*bActivate =*/ false);

	//check(!RopesConstraints.Find(Offset));  // We cannot already have this Offset in the map, particle ranges are always added, never removed (unless reset)
	
	// RopesConstraints.Emplace(Offset, MakeUnique<FRopeConstraints>())
	// 	->Initialize(Evolution.Get(), AnimationPositions, OldAnimationPositions, Offset, NumParticles);

	return Offset;
}
void FRopeSimulationSolver::Update(float InDeltaTime)
{
	Evolution->SetIterations(NumIterations);

	// Filter delta time to smoothen time variations and prevent unwanted vibrations
	static const float DeltaTimeDecay = 0.1f;	
	DeltaTime = DeltaTime + (InDeltaTime - DeltaTime) * DeltaTimeDecay;
	const float SubstepDeltaTime = DeltaTime / (float)NumSubsteps;
	
	for (int32 i = 0; i < NumSubsteps; ++i)
	{
		Evolution->AdvanceOneTimeStep(SubstepDeltaTime);
	}

	Time = Evolution->GetTime();
}

const TVector<float, 3>* FRopeSimulationSolver::GetParticlePs(int32 Offset) const
{
	return &Evolution->Particles().P(Offset);
}

TVector<float, 3>* FRopeSimulationSolver::GetParticlePs(int32 Offset)
{
	return &Evolution->Particles().P(Offset);
}
const TVector<float, 3>* FRopeSimulationSolver::GetParticleXs(int32 Offset) const
{
	return &Evolution->Particles().X(Offset);
}
const Quat* FRopeSimulationSolver::GetEdgeQs(int32 Offset) const
{
	Quat* ptr = &Evolution->Edges().Q(Offset);
	return ptr;
}
TVector<float, 3>* FRopeSimulationSolver::GetParticleXs(int32 Offset)
{
	return &Evolution->Particles().X(Offset);
}


const TVector<float, 3>* FRopeSimulationSolver::GetParticleVs(int32 Offset) const
{
	return &Evolution->Particles().V(Offset);
}

TVector<float, 3>* FRopeSimulationSolver::GetParticleVs(int32 Offset)
{
	return &Evolution->Particles().V(Offset);
}

const float* FRopeSimulationSolver::GetParticleInvMasses(int32 Offset) const
{
	return &Evolution->Particles().InvM(Offset);
}

void FRopeSimulationSolver::SetParticleMassUniform(int32 Offset,const CRProperty& Property)
{
	const int32 Size = Evolution->GetParticleRangeSize(Offset);
	TPBDParticles<PDScalar, 3>& Particles = Evolution->Particles();
	float UniformMass = Property.A*Property.l0*Property.Density/Size;
	float InvUniformMass = 1/UniformMass;
	
	for (int32 Index = Offset; Index < Offset + Size; ++Index)
	{
		Particles.M(Index) =UniformMass;
		Particles.InvM(Index) =InvUniformMass;
	}
	if(Property.EndM>0)
	{
		Particles.M(Offset + Size-1) = UniformMass + Property.EndM;
		Particles.InvM(Offset + Size-1) = 1/(UniformMass + Property.EndM);
	}
}

//TODO:add torque for start
void FRopeSimulationSolver::InitEdges(int32 Offset,const CRProperty& Property)
{
	const int32 Size = Evolution->GetEdgeRangeSize(Offset);
	TCosseratEdges<PDScalar, 3>& Edges = Evolution->Edges();
	PDScalar J1,J2,J3;
	J1 = J2 = PI*FMath::Pow(Property.r0,4)/4;
	J3 = 2*J1;
	Vec3 Jflat;
	Jflat<<J1,J2,J3;
	Jflat *=Property.l0*Property.Density;
	Mat3 UniformJ;
	UniformJ = Jflat.asDiagonal();
	Mat3 UniformJinv;
	UniformJinv = Vec3(1/Jflat.x(),1/Jflat.y(),1/Jflat.z()).asDiagonal();
	

	for (int32 Index = Offset; Index < Offset + Size; ++Index)
	{
		Edges.J(Index) =UniformJ;
		Edges.Jinv(Index) =UniformJinv;
		//Edges.Q(Index) = Quat(0,0,0,0);
		Edges.W(Index) = Vec3(0,0,0);
		Edges.Torque(Index) = Vec3(0,0,0);
	}
}
void FRopeSimulationSolver::SetEdgeOrientation(int32 PosOffset,int32 QuatOffset,CRProperty Property)
{
	const int32 PSize = Evolution->GetParticleRangeSize(PosOffset);
	TPBDParticles<PDScalar, 3>& Particles = Evolution->Particles();
	const int32 QSize = Evolution->GetEdgeRangeSize(QuatOffset);
	TCosseratEdges<PDScalar, 3>& Edges = Evolution->Edges();
	
	
	for (int32 Index = 0; Index <  QSize; ++Index)
	{
		int32 PIndex  = Index+PosOffset;
		int32 QIndex = Index+QuatOffset;
		auto _Delta = (Particles.X(PIndex+1)-Particles.X(PIndex+1));
		Vec3 Delta;
		Delta<<_Delta.X,_Delta.Y,_Delta.Z;
		Edges.Q(Index) = Quat::FromTwoVectors(Property.e3 ,Delta.normalized()).normalized();
	}
}

void FRopeSimulationSolver::UpdateStatus()
{
	Evolution->UpdateMatrix();
}


void FRopeSimulationSolver::EnableParticles(int32 Offset, bool bEnable)
{
	Evolution->ActivateParticleRange(Offset, bEnable);
	GetRopeConstraints(Offset).Enable(bEnable);
}
void FRopeSimulationSolver::EnableEdges(int32 Offset, bool bEnable)
{
	Evolution->ActivateEdgeRange(Offset, bEnable);
}

void FRopeSimulationSolver::SetAnimationPos(int32 Offset,TConstArrayView<TVector<float, 3>> InAnimationPos) 
{
	Swap(OldAnimationPositions, AnimationPositions);
	for(int32 i = Offset;i<Offset+InAnimationPos.Num();i++)
	{
		AnimationPositions[i] = InAnimationPos[i];
		Evolution->Particles().InvM(i) = 0;
	}
}
void FRopeSimulationSolver::SetParticleM(int32 Offset,PDScalar InM) 
{

	Evolution->Particles().M(Offset) = InM;
	if(InM==0)Evolution->Particles().InvM(Offset) = 0;
	else Evolution->Particles().InvM(Offset) = 1/InM;
	Evolution->UpdateMatrix();
	
}