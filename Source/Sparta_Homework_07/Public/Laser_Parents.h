#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Laser_Parents.generated.h"

class ALaserSpawner;
class ALaser;

UCLASS()
class SPARTA_HOMEWORK_07_API ALaser_Parents : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Laser|Components")
	USceneComponent* SceneRootComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Laser|Components")
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Laser|Properties")
	bool bActive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Laser|Properties")
	FLinearColor LaserColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Laser|Properties")
	ALaserSpawner* LaserSpawnerRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Laser|Properties")
	TArray<ALaser*> LaserAttached;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Laser|Properties")
	bool bLaserHit;
	
public:	
	ALaser_Parents();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SetLaserColor(FLinearColor Color);
	FLinearColor GetLaserColor() {return LaserColor;}
	
	void SetbActive(bool Active) {bActive = Active;}


	void SetLaserSpawnerRef(ALaserSpawner* Spawner);
	ALaserSpawner* GetLaserSpawner() {return LaserSpawnerRef;}
};
