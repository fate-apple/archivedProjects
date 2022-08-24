// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PDRopeConfig.h"
//#include "PDRopeSimulationSolver.h"

#include "RopeSimulationInterface.h"
#include "Chaos/Matrix.h"
#include "Components/ActorComponent.h"
#include "Components/MeshComponent.h"
#include "PDTypes.h"
#include "PDRopeComponent.generated.h"

#define lengthUnitConversion 0.01f
#define hardcoding_test1 

namespace Chaos
{
	class FRopeSimulationSolver;
	class FRopeSimulation;
}

class FParallelRopeTask;




class FPrimitiveSceneProxy;
using Vector3 = Chaos::TVector< float, 3 > ;
using Matrix3 = Chaos::PMatrix< float, 3, 3 >;
struct FRopeParticle {
	FRopeParticle() :
		bFree(true)
		, Position(0, 0, 0)
		, OldPosition(0, 0, 0)
	{}

	/** If this point is free (simulating) or fixed to something */
	bool bFree;
	/** 当前位置 */
	FVector Position;
	FVector Velocity;
	/** 上一次迭代的位置 */
	FVector OldPosition;
	float m;
	
};

/** Component that allows you to specify custom triangle mesh geometry */

UCLASS(hidecategories = (Object, Physics, Activation, "Components|Activation"), editinlinenew, meta = (BlueprintSpawnableComponent), ClassGroup = Rendering)
class PD_API UPDRopeComponent : public UMeshComponent
{
	GENERATED_UCLASS_BODY()
	friend FParallelRopeTask;

	
public:
	// Sets default values for this component's properties
	UPDRopeComponent();
	//~ Begin UActorComponent Interface.
	virtual void OnRegister() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void SendRenderDynamicData_Concurrent() override;
	virtual void CreateRenderState_Concurrent(FRegisterComponentContext* Context) override;
	virtual void ApplyWorldOffset(const FVector& InOffset, bool bWorldShift) override;
	//~ End UActorComponent Interface.

	//~ Begin USceneComponent Interface.
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	virtual void QuerySupportedSockets(TArray<FComponentSocketDescription>& OutSockets) const override;
	virtual bool HasAnySockets() const override;
	virtual bool DoesSocketExist(FName InSocketName) const override;
	virtual FTransform GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace = RTS_World) const override;
	virtual void OnVisibilityChanged() override;
	//~ End USceneComponent Interface.

	//~ Begin UPrimitiveComponent Interface.
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	//~ End UPrimitiveComponent Interface.

	//~ Begin UMeshComponent Interface.
	virtual int32 GetNumMaterials() const override;
	//~ End UMeshComponent Interface.



	/**
	 *	Should we fix the start to something, or leave it free.
	 *	If false, component transform is just used for initial location of start of cable
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerletRope")
		bool bAttachStartV;
	/**
	 *	Should we fix the end to something (using AttachEndTo and EndLocation), or leave it free.
	 *	If false, AttachEndTo and EndLocation are just used for initial location of end of cable
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerletRope")
		bool bAttachEndV;

	/** Actor or Component that the defines the end position of the cable */
	UPROPERTY(EditAnywhere, Category = "VerletRope")
		FComponentReference AttachEndToV;

	/** Socket name on the AttachEndTo component to attach to */
	UPROPERTY(EditAnywhere, Category = "VerletRope")
		FName AttachEndToSocketNameV;
	/** End location of cable, relative to AttachEndTo (or AttachEndToSocketName) if specified, otherwise relative to cable component. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerletRope", meta = (MakeEditWidget = true))
		FVector EndLocationV;
	/** Attaches the end of the cable to a specific Component **/
	UFUNCTION(BlueprintCallable, Category = "VerletRope")
		void SetAttachEndToComponentV(USceneComponent* Component, FName SocketName = NAME_None);

	/** Attaches the end of the cable to a specific Component within an Actor **/
	UFUNCTION(BlueprintCallable, Category = "VerletRope")
		void SetAttachEndToV(AActor* Actor, FName ComponentProperty, FName SocketName = NAME_None);
	

	/** Gets the Actor that the cable is attached to **/
	UFUNCTION(BlueprintCallable, Category = "VerletRope")
		AActor* GetAttachedActorV() const;

	/** Gets the specific USceneComponent that the cable is attached to **/
	UFUNCTION(BlueprintCallable, Category = "VerletRope")
		USceneComponent* GetAttachedComponentV() const;

	/** Get array of locations of particles (in world space) making up the cable simulation. */
	UFUNCTION(BlueprintCallable, Category = "VerletRope")
		void GetCableParticleLocationsV(TArray<FVector>& Locations) const;



	/** When false, will still wait for SubstepTime to elapse before updating, but will only run the cable simulation once using all of accumulated simulation time */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "VerletRope")
		bool bUseSubsteppingV = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "VerletRope")
		bool bSkipCableUpdateWhenNotVisibleV = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "VerletRope")
		bool bSkipCableUpdateWhenNotOwnerRecentlyRenderedV = false;

	/** Rest length of the Rope */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerletRope", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1000.0"))
		float RopeLengthV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerletRope", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "100.0"))
	float r0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerletRope", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "100.0"))
	float density;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerletRope", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float E;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerletRope", meta = (ClampMin = "0.0", UIMin = "1.0"))
	float EndM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerletRope", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float PoissonRatio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerletRope")
	bool pauseSimulation=true;
	UFUNCTION(BlueprintCallable, Category = "VerletRope")
		void SetPauseSimulation(){pauseSimulation=(!pauseSimulation);};


	/** Add stiffness constraints to cable. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "VerletRope")
		bool bEnableStiffnessV;

	/** Controls the simulation substep time for the cable */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerletRope", meta = (UIMin = "0.000001", UIMax = "0.1"))
		float SubstepTimeV;
	/** How many segments the cable has */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VerletRope", meta = (ClampMin = "1", UIMin = "1", UIMax = "20"))
		int32 NumSegmentsV;
	




	/**
	 *	EXPERIMENTAL. Perform sweeps for each cable particle, each substep, to avoid collisions with the world.
	 *	Uses the Collision Preset on the component to determine what is collided with.
	 *	This greatly increases the cost of the cable simulation.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "VerletRope")
		bool bEnableCollisionV;

	/** If collision is enabled, control how much sliding friction is applied when cable is in contact. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "VerletRope", meta = (ClampMin = "0.0", ClampMax = "1.0", EditCondition = "bEnableCollision"))
		float CollisionFrictionV;

	/** Force vector (world space) applied to all particles in cable. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerletRope Forces")
		FVector RopeForceV;

	/** Scaling applied to world gravity affecting this cable. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerletRope Forces")
		float RopeGravityScaleV;

	/** How wide the cable geometry is */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerletRope Rendering", meta = (ClampMin = "0.01", UIMin = "0.01", UIMax = "50.0"))
		float RopeWidthV;

	/** Number of sides of the cable geometry */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VerletRope Rendering", meta = (ClampMin = "1", ClampMax = "16"))
		int32 NumSidesV;

	/** How many times to repeat the material along the length of the cable */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerletRope Rendering", meta = (UIMin = "0.1", UIMax = "8"))
		float TileMaterialV;


	/** The number of solver iterations controls how 'stiff' the cable is */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VerletRope", meta = (ClampMin = "1", ClampMax = "16"))
		int32 SolverIterationsV;

	void RecreateRopeActors();
	void Update(Chaos::FRopeSimulationSolver* Solver, int32 PrevLODIndex, int32 LODIndex, int32 PrevOffset,
	            int32 Offset);
	void SkinPhysicsMesh(int32 LODIndex,
	                     Chaos::TVector<float, 3>* OutPositionss) const;
	UPDRopeSharedSimConfig* GetSharedClothConfig();

	//non-LOD for now
	int32 GetNumLODs(){return 1;}
	int32 GetNumPoints()
	{
		//return Vertices.Num();
		return Particles.Num();
	}
	int32 GetLODIndex(){return 0;}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void WritebackClothingSimulationData();
	void SetAnimationPos(int Index, FVector Pos);
	void SetParticleM(int Index, PDScalar InM);


private:
	/** Perform collision traces for particles */
	void SolveCollisionConstraints();
	/** Perform a simulation substep */
	void SolveSubstep(float InSubstepTime, const FVector& Gravity);
	/** Get start and end position for the cable */
	void GetEndPositions(FVector& OutStartPosition, FVector& OutEndPosition);
	void Reset();

	
	/** Amount of time 'left over' from last tick */
	float TimeRemainder;

	/*绳子粒子数组*/
	TArray<FRopeParticle> Particles;
	//TArray< FVector> lengths;
	float L0,A0,I0_1;
	


	TArray<Chaos::TVector<float, 3>> Vertices;
	TSubclassOf<class URopeSimulationFactory> RopeSimulationFactory;
	Chaos::FRopeSimulation* RopeSimulation;
	FGraphEventRef ParallelRopeTask;
	TMap<int32, FRopeSimulData> CurrentSimulationData;
	
	//TODO: GetCOntext()
	public:
	float DeltaSeconds;
	Vec3 e3;
	//FVector RopeStart, RopeEnd;

	
	friend class FCableSceneProxy;
};
