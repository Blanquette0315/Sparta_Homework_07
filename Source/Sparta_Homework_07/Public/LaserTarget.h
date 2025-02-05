#pragma once

#include "CoreMinimal.h"
#include "LaserInterface.h"
#include "Laser_Parents.h"
#include "SelectableInterface.h"
#include "LaserTarget.generated.h"

UCLASS()
class SPARTA_HOMEWORK_07_API ALaserTarget : public ALaser_Parents, public ILaserInterface, public ISelectableInterface
{
	GENERATED_BODY()


	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserTarget|Properties")
	FLinearColor BasicColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserTarget|Properties")
	SELECTABLE_TYPE SelectableType;
	
public:	
	ALaserTarget();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void PuzzleSolved(FLinearColor HitLaserColor);

	virtual ALaser* LaserBounce(ALaser* LaserHit, bool HitResponse, const FHitResult& HitInformation) override;

	virtual void Select() override {};
	virtual void SetRotationSpeed(FRotator RotationSpeed) override {};
	virtual SELECTABLE_TYPE GetSelectableType() override {return SelectableType;}
};
