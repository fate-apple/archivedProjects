

#pragma once
#include "RopeSimulationInterface.h"
#include "RopeSimulationFactory.generated.h"
UCLASS(Abstract)
class PD_API URopeSimulationFactory  : public UObject
{
	GENERATED_BODY()

public:
	// Return the default clothing simulation factory class as set by the build or by
	// the p.Cloth.DefaultClothingSimulationFactoryClass console variable if any available.
	// Otherwise return the last registered factory.
	//static TSubclassOf<class URopeSimulationFactory> GetDefaultRopeSimulationFactoryClass();

	// Create a simulation object for a skeletal mesh to use (see IClothingSimulation)
	virtual IRopeSimulation* CreateSimulation()
	PURE_VIRTUAL(URopeSimulationFactory::CreateSimulation, return nullptr;);

	// Destroy a simulation object, guaranteed to be a pointer returned from CreateSimulation for this factory
	virtual void DestroySimulation(IRopeSimulation* InSimulation)
	PURE_VIRTUAL(URopeSimulationFactory::DestroySimulation, );
};