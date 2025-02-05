#pragma once

#include "CoreMinimal.h"
#include "LaserInterface.h"
#include "Laser_Parents.h"
#include "SelectableInterface.h"
#include "Components/ArrowComponent.h"
#include "LaserReflection.generated.h"

/**
 * 
 */
UCLASS()
class SPARTA_HOMEWORK_07_API ALaserReflection : public ALaser_Parents, public ILaserInterface, public ISelectableInterface
{
	GENERATED_BODY()

private:
	SELECTABLE_TYPE SelectableType;
	FRotator SelectRotationSpeed;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserReflection|Components")
	UArrowComponent* ArrowComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LaserSpawner|Properties" , Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ALaser> BPLaserRef;
	
public:	
	ALaserReflection();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	FRotator CalRotation(const FVector& ImpactPoint, const FVector& ImpactNormal, const FVector& TraceStart) const;

	void ClearAttachedLasers(const FString& ClearLaserID);
	virtual ALaser* LaserBounce(ALaser* LaserHit, bool HitResponse, const FHitResult& HitInformation) override;

	virtual SELECTABLE_TYPE GetSelectableType() override;
	virtual void Select() override;
	virtual void SetRotationSpeed(FRotator RotationSpeed) override;
};