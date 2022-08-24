#include "PDRopeSimulationFactory.h"

#include "PDRopeSimulation.h"
using namespace Chaos;
FRopeSimulation*  UPDRopeSimulationFactory::CreateSimulation()
{
	FRopeSimulation* Simulation = new Chaos::FRopeSimulation();
	return Simulation;
}

void UPDRopeSimulationFactory::DestroySimulation(FRopeSimulation* InSimulation)
{
	delete InSimulation;
}
