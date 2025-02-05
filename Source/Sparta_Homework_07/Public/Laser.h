// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "LaserInterface.h"
#include "Laser_Parents.h"
#include "Laser.generated.h"

/**
 * 
 */
UCLASS()
class SPARTA_HOMEWORK_07_API ALaser : public ALaser_Parents//, public ILaserInterface
{
	GENERATED_BODY()

private:
	FHitResult HitResult;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Laser|Properties")
	AActor* LastHitActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Laser|Properties")
	AActor* CurrentHitActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Laser|Properties")
	FString LaserID;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Laser|Properties")
	ALaser* BounceLaser;
	
public:	
	ALaser();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	void ChangeColor() const;
	void SetLaserEndPoint();
	FString GetLaserID() const;
	void SetLaserID(const FString& ID);

	bool CancelTargetHit(bool IsOverride);
	//virtual ALaser* LaserBounce(ALaser* LaserHit, bool HitResponse, const FHitResult& HitInformation) override {return nullptr;} ;

	void UpdateLaserID(const FString& strAppend);
};
