#pragma once

#include "CoreMinimal.h"
#include "Laser_Parents.h"
#include "Components/ArrowComponent.h"
#include "LaserSpawner.generated.h"

/**
 * 
 */
UCLASS()
class SPARTA_HOMEWORK_07_API ALaserSpawner : public ALaser_Parents
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LaserSpawner|Properties" , Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ALaser> BPLaserRef;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LaserSpawner|Properties")
	TArray<ALaser*> ChildLasers;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LaserSpawner|Properties")
	FString ID;
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "LaserSpawner|Properties")
	ALaser* SpawnedLaser;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "LaserSpawner|Components")
	UArrowComponent* SpawnLaserArrow;
	
public:	
	ALaserSpawner();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void RemoveFollowingLaser(ALaser* LaserRef);
	void ClearAllLaserAtteched();

	void AddLaserChild(ALaser* LaserRef);
};
