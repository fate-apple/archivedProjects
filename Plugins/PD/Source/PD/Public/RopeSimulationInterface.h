
#pragma once
class  UPDRopeComponent;
class PD_API IRopeSimulation
{
	public:
	IRopeSimulation();
	virtual ~IRopeSimulation();

	virtual void Initialize() = 0;

	virtual void CreateActor(UPDRopeComponent* InOwnerComponent, int32 InSimDataIndex)=0;
};
