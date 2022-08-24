#pragma once
namespace Chaos
{
	class FRopeSimulation;
	
	class PD_API UPDRopeSimulationFactory final  : public UObject
{
	public:
	FRopeSimulation* CreateSimulation();
	void DestroySimulation(FRopeSimulation* InSimulation);

};
}