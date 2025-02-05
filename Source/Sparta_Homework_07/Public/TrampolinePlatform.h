#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrampolinePlatform.generated.h"

UCLASS()
class SPARTA_HOMEWORK_07_API ATrampolinePlatform : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TrampolinePlatform|Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TrampolinePlatform|Properties")
	float Force;
	
public:	
	ATrampolinePlatform();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="FallPlatform|Functions")
	void OnPlatformHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
