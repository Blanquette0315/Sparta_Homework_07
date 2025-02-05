#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LaserInterface.generated.h"

class ALaser;

UINTERFACE(MinimalAPI)
class ULaserInterface : public UInterface
{
	GENERATED_BODY()
};

class SPARTA_HOMEWORK_07_API ILaserInterface
{
	GENERATED_BODY()

public:
	virtual ALaser* LaserBounce(ALaser* LaserHit, bool HitResponse, const FHitResult& HitInformation) = 0;
};