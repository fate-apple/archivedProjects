// Fill out your copyright notice in the Description page of Project Settings.

#include "PDRopeComponent.h"


#include <vector>
#include <hlslcc/hlslcc/src/hlslcc_lib/compiler.h>


#include "AnimationCoreLibrary.h"
#include "EngineGlobals.h"
#include "PrimitiveViewRelevance.h"
#include "RenderResource.h"
#include "RenderingThread.h"
#include "WorldCollision.h"
#include "PrimitiveSceneProxy.h"
#include "VertexFactory.h"
#include "MaterialShared.h"
#include "SceneManagement.h"
#include "Engine/CollisionProfile.h"
#include "Materials/Material.h"
#include "LocalVertexFactory.h"
#include "Engine/Engine.h"

#include "DynamicMeshBuilder.h"
#include "StaticMeshResources.h"
#include "HAL/UnrealMemory.h"

#include "CableSceneProxy.h"
#include "PDRopeSimulation.h"
#include "PDRopeSimulationSolver.h"
#include "PDRopeSimulationFactory.h"
#include "RopeSimulationFactory.h"
//#include "Math/UnrealMathNeon.h"
DECLARE_STATS_GROUP(TEXT("PDRope"), STATGROUP_PDRopeComponent, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("VerletRope Sim"), STAT_VerletRope_SimTime, STATGROUP_PDRopeComponent);
DECLARE_CYCLE_STAT(TEXT("VerletRope Solve"), STAT_VerletRope_SolveTime, STATGROUP_PDRopeComponent);
DECLARE_CYCLE_STAT(TEXT("VerletRope Collision"), STAT_VerletRope_CollisionTime, STATGROUP_PDRopeComponent);
DECLARE_CYCLE_STAT(TEXT("VerletRope Integrate"), STAT_VerletRope_IntegrateTime, STATGROUP_PDRopeComponent);

static FName CableEndSocketName(TEXT("CableEnd"));
static FName CableStartSocketName(TEXT("CableStart"));


//temp for now
UPDRopeSharedSimConfig* UPDRopeComponent::GetSharedClothConfig()
{
	static UPDRopeSharedSimConfig  config;
	return &config;
}
class FParallelRopeTask
{
	const UPDRopeComponent* PDRopeComponent;
	float DeltaTime;
	

public:
	FParallelRopeTask(const UPDRopeComponent* InPDRopeComponent, float InDeltaTime)
		: PDRopeComponent(InPDRopeComponent)
		, DeltaTime(InDeltaTime)
	{
	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FParallelClothTask, STATGROUP_TaskGraphTasks);
	}
	static ENamedThreads::Type GetDesiredThread()
	{
		return ENamedThreads::GameThread;
	}
	static ESubsequentsMode::Type GetSubsequentsMode()
	{
		return ESubsequentsMode::TrackSubsequents;
	}
	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
	{
		// Perform the data writeback
		if(PDRopeComponent->RopeSimulation)
		{
			PDRopeComponent->RopeSimulation->Simulate(PDRopeComponent);
		}
	}
};

//////////////////////////////////////////////////
// Sets default values for this component's properties
UPDRopeComponent::UPDRopeComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bTickInEditor = true;
	bAutoActivate = true;

	RopeWidthV =10.f;
	NumSegmentsV = 30;
	NumSidesV = 4;
	EndLocationV = FVector(0.f, 0, 0);
	RopeLengthV = 100.f;
	SubstepTimeV = 0.02f;
	SolverIterationsV = 1;
	TileMaterialV = 1.f;
	CollisionFrictionV = 0.2f;
	RopeGravityScaleV = 1.f;
	r0 = 0.25f;
	density = 10.f;
	SubstepTimeV=0.0001f;
	E=5000;
	PoissonRatio = 0.5;


	SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);

	// ...
}

FPrimitiveSceneProxy* UPDRopeComponent::CreateSceneProxy()
{
	return new FCableSceneProxy(this);
}
int32 UPDRopeComponent::GetNumMaterials() const
{
	return 1;
}
void UPDRopeComponent::OnRegister() {
	Super::OnRegister();
	if(!RopeSimulationFactory)
	{
		RopeSimulationFactory = TSubclassOf< URopeSimulationFactory>();
	}
	Reset();
	RecreateRopeActors();
	if(RopeSimulation)
	{
		RopeSimulation->GetSimulationData(CurrentSimulationData);
	}
}
 
void UPDRopeComponent::Reset()
{
	const int32 NumParticles = NumSegmentsV + 1;
	FVector RopeStart, RopeEnd;
	GetEndPositions(RopeStart, RopeEnd);
	const FVector Delta = (RopeEnd - RopeStart);
	Particles.Reset();
	Particles.AddUninitialized(NumParticles);

	for (int index = 0; index < NumParticles; index++) {
		FRopeParticle & particle = Particles[index];
		const float alpha = (float)index / (float)NumSegmentsV;
		const FVector InitialPosition = RopeStart + (alpha*Delta);
		particle.Position = InitialPosition;
		particle.OldPosition = InitialPosition;
		particle.bFree = true;
		particle.Euler = FVector(0,0,0);
	}
	e3<<Delta.X,Delta.Y,Delta.Z;
	e3.normalize();
}




void UPDRopeComponent::SolveCollisionConstraints() {
	

}


void UPDRopeComponent::SolveSubstep(float InSubstepTime, const FVector& Gravity)
{
	//待定
	SCOPE_CYCLE_COUNTER(STAT_VerletRope_SimTime);

	//SolveDistanceConstraints();

	if (bEnableCollisionV) {
		SolveCollisionConstraints();
	}
}




void UPDRopeComponent::SetAttachEndToComponentV(USceneComponent* Component, FName SocketName)
{
	AttachEndToV.OtherActor = Component->GetOwner();
	AttachEndToV.ComponentProperty = NAME_None;
	AttachEndToV.OverrideComponent = Component;
	AttachEndToSocketNameV = SocketName;
}

void UPDRopeComponent::SetAttachEndToV(AActor* Actor, FName ComponentProperty, FName SocketName)
{
	AttachEndToV.OtherActor = Actor;
	AttachEndToV.ComponentProperty = ComponentProperty;
	AttachEndToSocketNameV = SocketName;
}

AActor* UPDRopeComponent::GetAttachedActorV() const
{
	return AttachEndToV.OtherActor;
}

USceneComponent* UPDRopeComponent::GetAttachedComponentV() const
{
	return Cast<USceneComponent>(AttachEndToV.GetComponent(GetOwner()));
}

void UPDRopeComponent::GetCableParticleLocationsV(TArray<FVector>& Locations) const
{
	Locations.Empty();
	for (const FRopeParticle& Particle : Particles)
	{
		Locations.Add(Particle.Position);
	}
}

// Called when the game starts
void UPDRopeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

// Called every frame
void UPDRopeComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//UE_LOG(LogTemp, Warning, TEXT("tICK"));
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//TODO:generate rope simulation context;
	this->DeltaSeconds = DeltaTime;
	if (bSkipCableUpdateWhenNotVisibleV && !IsVisible())
	{
		SetComponentTickEnabled(false);
		return;
	}

	AActor* Owner = GetOwner();
	if (bSkipCableUpdateWhenNotOwnerRecentlyRenderedV && Owner && !Owner->WasRecentlyRendered(2.0f))
	{
		return;
	}
	
	FVector RopeStart, RopeEnd;
	GetEndPositions(RopeStart, RopeEnd);


	FRopeParticle & StartParticle = Particles[0];

	if (bAttachStartV) {
		StartParticle.Position = StartParticle.OldPosition = RopeStart;
		StartParticle.bFree = false;
		CurrentSimulationData[0].Positions[0] = RopeStart;
		SetAnimationPos(0,RopeStart);
	}
	else {
		StartParticle.bFree = true;
	}
	FRopeParticle& EndParticle = Particles[NumSegmentsV];
	if (bAttachEndV) {
		EndParticle.Position = EndParticle.OldPosition = RopeEnd;
		EndParticle.bFree = false;
		//SetParticleM(NumSegmentsV,EndM);
	}
	else {
		EndParticle.bFree = true;
	}
	//check(IsInGameThread());
	//ParallelRopeTask = TGraphTask<FParallelRopeTask>::CreateTask(nullptr, ENamedThreads::GameThread).ConstructAndDispatchWhenReady(this, DeltaTime);
	if(this->RopeSimulation&&!pauseSimulation)
	{
		this->RopeSimulation->Simulate(this);
		WritebackRopeSimulationData();
	}
	
	if (bAttachEndV)
	{
		SetAttachEndObject();
	}
	
	// Need to send new data to render thread
	MarkRenderDynamicDataDirty();

	// Call this because bounds have changed
	UpdateComponentToWorld();


}

void UPDRopeComponent::CreateRenderState_Concurrent(FRegisterComponentContext* Context)
{
	Super::CreateRenderState_Concurrent(Context);

	SendRenderDynamicData_Concurrent();
}


void UPDRopeComponent::ApplyWorldOffset(const FVector & InOffset, bool bWorldShift)
{
	Super::ApplyWorldOffset(InOffset, bWorldShift);

	for (FRopeParticle& Particle : Particles)
	{
		Particle.Position += InOffset;
		Particle.OldPosition += InOffset;
	}
}


void UPDRopeComponent::SendRenderDynamicData_Concurrent()
{
	if (SceneProxy)
	{
		// Allocate cable dynamic data
		FRopeDynamicData* DynamicData = new FRopeDynamicData;

		// Transform current positions from particles into component-space array
		const FTransform& ComponentTransform = GetComponentTransform();
		int32 NumPoints = NumSegmentsV + 1;
		DynamicData->Points.AddUninitialized(NumPoints);
		DynamicData->Eulers.AddUninitialized(NumPoints);
		for (int32 PointIdx = 0; PointIdx < NumPoints; PointIdx++)
		{
			DynamicData->Points[PointIdx] = ComponentTransform.InverseTransformPosition(Particles[PointIdx].Position);
			DynamicData->Eulers[PointIdx] = Particles[PointIdx].Euler;
		}

		// Enqueue command to send to render thread
		FCableSceneProxy* CableSceneProxy = (FCableSceneProxy*)SceneProxy;
		ENQUEUE_RENDER_COMMAND(FSendCableDynamicData)(
			[CableSceneProxy, DynamicData](FRHICommandListImmediate& RHICmdList)
		{
			CableSceneProxy->SetDynamicData_RenderThread(DynamicData);
		});
	}
}
void UPDRopeComponent::SetAttachEndObject()
{
	UPrimitiveComponent* EndComponent = Cast<UPrimitiveComponent>(AttachEndToV.GetComponent(GetOwner()));
	if (EndComponent != NULL)
	{
		FRotator r =  FRotator(FQuatFromQuat(CurrentSimulationData[0].Quats[Particles.Num()-2]));
		r = FRotator(0,EulerZFromQuat(CurrentSimulationData[0].Quats[Particles.Num()-2])*180,0);
		//EndComponent->SetRelativeRotation(r);
		EndComponent->SetRelativeRotation(r);
	}
}
void UPDRopeComponent::GetEndPositions(FVector& OutStartPosition, FVector& OutEndPosition) {
	OutStartPosition = GetComponentLocation();
	// See if we want to attach the other end to some other component
	UPrimitiveComponent* EndComponent = Cast<UPrimitiveComponent>(AttachEndToV.GetComponent(GetOwner()));
	if (EndComponent == NULL)
	{
		EndComponent = this;
		//EndComponent->AddTorque()
		
	}

	if (AttachEndToSocketNameV != NAME_None)
	{
		OutEndPosition = EndComponent->GetSocketTransform(AttachEndToSocketNameV).TransformPosition(EndLocationV);
	}
	else
	{
		OutEndPosition = EndComponent->GetComponentTransform().TransformPosition(EndLocationV);
		//EndComponent->SetComponentToWorld();
	}
}


void UPDRopeComponent::OnVisibilityChanged()
{
	Super::OnVisibilityChanged();

	// Does not interact well with any other states that would be blocking tick
	if (bSkipCableUpdateWhenNotVisibleV)
	{
		SetComponentTickEnabled(IsVisible());
	}
}

FBoxSphereBounds UPDRopeComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	// Calculate bounding box of cable points
	FBox RopeBox(ForceInit);
	for (int32 ParticleIdx = 0; ParticleIdx < Particles.Num(); ParticleIdx++)
	{
		const FRopeParticle& Particle = Particles[ParticleIdx];
		RopeBox += Particle.Position;
	}

	// Expand by cable radius (half cable width)
	return FBoxSphereBounds(RopeBox.ExpandBy(0.5f * RopeWidthV));
}

void UPDRopeComponent::QuerySupportedSockets(TArray<FComponentSocketDescription>& OutSockets) const
{
	OutSockets.Add(FComponentSocketDescription(CableEndSocketName, EComponentSocketType::Socket));
	OutSockets.Add(FComponentSocketDescription(CableStartSocketName, EComponentSocketType::Socket));
}

FTransform UPDRopeComponent::GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace) const
{
	int32 NumParticles = Particles.Num();
	if ((InSocketName == CableEndSocketName || InSocketName == CableStartSocketName) && NumParticles >= 2)
	{
		FVector ForwardDir, Pos;
		if (InSocketName == CableEndSocketName)
		{
			FVector LastPos = Particles[NumParticles - 1].Position;
			FVector PreviousPos = Particles[NumParticles - 2].Position;

			ForwardDir = (LastPos - PreviousPos).GetSafeNormal();
			Pos = LastPos;
		}
		else
		{
			FVector FirstPos = Particles[0].Position;
			FVector NextPos = Particles[1].Position;

			ForwardDir = (NextPos - FirstPos).GetSafeNormal();
			Pos = FirstPos;
		}

		const FQuat RotQuat = FQuat::FindBetween(FVector(1, 0, 0), ForwardDir);
		FTransform WorldSocketTM = FTransform(RotQuat, Pos, FVector(1, 1, 1));

		switch (TransformSpace)
		{
		case RTS_World:
		{
			return WorldSocketTM;
		}
		case RTS_Actor:
		{
			if (const AActor* Actor = GetOwner())
			{
				return WorldSocketTM.GetRelativeTransform(GetOwner()->GetTransform());
			}
			break;
		}
		case RTS_Component:
		{
			return WorldSocketTM.GetRelativeTransform(GetComponentTransform());
		}
		}
	}

	return Super::GetSocketTransform(InSocketName, TransformSpace);
}

bool UPDRopeComponent::HasAnySockets() const
{
	return (Particles.Num() >= 2);
}

bool UPDRopeComponent::DoesSocketExist(FName InSocketName) const
{
	return (InSocketName == CableEndSocketName) || (InSocketName == CableStartSocketName);
}

using namespace  Chaos;
void UPDRopeComponent::RecreateRopeActors()
{
	//UClass* SimFactoryClass = *RopeSimulationFactory;
	UClass* SimFactoryClass =  UPDRopeSimulationFactory::StaticClass();
	if (SimFactoryClass)
	{
		UPDRopeSimulationFactory* SimFactory = SimFactoryClass->GetDefaultObject<UPDRopeSimulationFactory>();
		
		//if(RopeSimulation == nullptr)
		{
			RopeSimulation = SimFactory->CreateSimulation();
		}
		if(RopeSimulation)
		{
			RopeSimulation->Initialize();
			RopeSimulation->CreateActor(this,0);	//BaseAssetIndex
		}
		//now add endM to Crproperty
		// if (bAttachEndV) {
		// 	SetParticleM(NumSegmentsV,EndM);
		// }
	}
}

void UPDRopeComponent::Update(
	FRopeSimulationSolver* Solver,
	int32 PrevLODIndex,
	int32 LODIndex,
	int32 PrevOffset,
	int32 Offset)
{
	check(Solver);

	TVector<float, 3>* const OutPositions = Solver->GetAnimationPositions(Offset);
	
	SkinPhysicsMesh(LODIndex, OutPositions);
}

void UPDRopeComponent::SkinPhysicsMesh(int32 LODIndex, TVector<float, 3>* OutPositions) const
{
	const uint32 NumPoints = Particles.Num();
	FMemory::Memzero((uint8*)OutPositions, NumPoints * sizeof(TVector<float, 3>));
	ParallelFor(NumPoints, [this,&OutPositions](uint32 VertIndex)
	{
		OutPositions[VertIndex] = Particles[VertIndex].Position;
	}, EParallelForFlags::None);
}


void UPDRopeComponent::WritebackRopeSimulationData()
{
	
	if(RopeSimulation)
	{
		RopeSimulation->GetSimulationData(CurrentSimulationData);
	}
	//TODO
	for(int32 i=0;i<Particles.Num();i++)
	{
		Particles[i].Position = CurrentSimulationData[0].Positions[i];
		Vec3 euler;
		PDScalar EulerZ;
		if(i==0)
		{
			euler = CurrentSimulationData[0].Quats[i].toRotationMatrix().eulerAngles(0, 1, 2);
			EulerZ = EulerZFromQuat(CurrentSimulationData[0].Quats[i]);
		}
		else
		{
			euler =  CurrentSimulationData[0].Quats[i-1].toRotationMatrix().eulerAngles(0, 1, 2);
			EulerZ = EulerZFromQuat(CurrentSimulationData[0].Quats[i-1]);
		}
		Particles[i].Euler = FVector(euler[0],euler[1],EulerZ);
		
	}
}
 
void UPDRopeComponent::SetAnimationPos(int Index,FVector Pos )
{
	auto InAnimationPos = MakeArrayView(const_cast<const Chaos::TVector<float, 3> *> (&CurrentSimulationData[0].Positions[0]+Index), 1);

	if(RopeSimulation)
	{
		RopeSimulation->SetAnimationPos(0,InAnimationPos);
	}
	//TODO
}

void UPDRopeComponent::SetParticleM(int Index,PDScalar InM )
{


	if(RopeSimulation)
	{
		RopeSimulation->SetParticleM(0,Index,InM);
	}
	//TODO
}