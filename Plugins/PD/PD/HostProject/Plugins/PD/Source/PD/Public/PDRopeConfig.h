#pragma once
#include "PDRopeConfig.generated.h"
UCLASS()
class  UPDRopeSharedSimConfig: public UObject
{

	GENERATED_BODY()
public:
	UPDRopeSharedSimConfig();
	virtual ~UPDRopeSharedSimConfig() ;
	UPROPERTY(EditAnywhere, Category = Simulation, meta = (UIMin = "1", UIMax = "10", ClampMin = "0", ClampMax = "100"))
	int32 IterationCount = 2;
	int32 SubdivisionCount =1;
};
