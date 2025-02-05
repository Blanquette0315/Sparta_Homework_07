#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnCharacter.generated.h"

class ASpartaCharacter;
class ADronePawn;
class UCapsuleComponent;
struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SPARTA_HOMEWORK_07_API APawnCharacter : public APawn
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character|Component")
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character|Component")
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera|Component")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera|Component")
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Property")
	float	JumpForce;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character|Property")
	float CurrentSpeed;
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category="Character|Property")
	bool	bIsGround;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character|Property")
	FVector TotalVelocity;
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category="Character|Property")
	bool	bIsFalling;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character|Property")
	TSubclassOf<ADronePawn> BPDroneRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character|Property")
	TSubclassOf<ASpartaCharacter> BPSpartaRef;
	
private:
	bool bMove;
	bool bPrevMove;
	
	float	NormalSpeed;
	float	SprintSpeedMultiplier;
	float	SprintSpeed;
	float	AirSpeedMultiplier;

	float	MaxWalkSpeed;

	float	Gravity;
	float	JumpSpeed;
	bool	bPressedJump;
	bool	bJumpFirst;
	
	FVector PreviousLocation;
	
	void CheckGround();
	void Landed();
	FVector CalNewLocation(float DeltaTime);
	
public:
	APawnCharacter();

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
	void ChangePlayer(const FInputActionValue& value);
};
