#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpartaCharacter.generated.h"

class UPhysicsHandleComponent;
class ADronePawn;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;

UCLASS()
class SPARTA_HOMEWORK_07_API ASpartaCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Component")
	UPhysicsHandleComponent* PhysicsHandleComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character|Property")
	TSubclassOf<ADronePawn> BPDroneRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Property")
	float GrabLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Property")
	float HoldDistance;
	
private:
	float NormalSpeed;
	float SprintSpeedMultiplier;
	float SprintSpeed;

	bool bIsGrabbing;
	
public:
	ASpartaCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	UFUNCTION()
	void SpawnDrone(const FInputActionValue& value);
	UFUNCTION()
	void Grab(const FInputActionValue& value);
	UFUNCTION()
	void SelectRot(const FInputActionValue& value);
};