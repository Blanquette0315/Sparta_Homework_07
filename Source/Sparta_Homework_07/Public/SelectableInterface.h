#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SelectableInterface.generated.h"

UENUM(BlueprintType)
enum class SELECTABLE_TYPE : uint8
{
	GRAB,
	GRAB_HOLDROT,
	ROTATE,
	BTN,

	END
};

UINTERFACE(MinimalAPI)
class USelectableInterface : public UInterface
{
	GENERATED_BODY()
};

class SPARTA_HOMEWORK_07_API ISelectableInterface
{
	GENERATED_BODY()

public:
	virtual SELECTABLE_TYPE GetSelectableType() = 0;
	virtual void Select() = 0;
	virtual void SetRotationSpeed(FRotator RotationSpeed) = 0;
};
