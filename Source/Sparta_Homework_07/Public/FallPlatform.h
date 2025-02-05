#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "FallPlatform.generated.h"

UCLASS()
class SPARTA_HOMEWORK_07_API AFallPlatform : public AActor
{
	GENERATED_BODY()

private:
	bool IsFalling;
	FTransform OriginTransform;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FallPlatform|Component")
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FallPlatform|Component")
	UBoxComponent* BoxComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="FallPlatform|Properties")
	float HoldingTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="FallPlatform|Properties")
	float ResetTime;

	FTimerHandle FallingTimerHandle;
	
public:	
	AFallPlatform();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category="FallPlatform|Functions")
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION(BlueprintCallable, Category="FallPlatform|Functions")
	//void OnPlatformHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION(BlueprintCallable, Category="FallPlatform|Functions")
	void FallingPlatform();

	UFUNCTION(BlueprintCallable, Category="FallPlatform|Functions")
	void ResetPlatform();

	void SetHolingTime(const float Time) { HoldingTime = Time; }
	void SetResetTime(const float Time) { ResetTime = Time; }
};
