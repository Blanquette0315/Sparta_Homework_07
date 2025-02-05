#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DronePawn.generated.h"

class UPhysicsHandleComponent;
class UArrowComponent;
struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;
class UCapsuleComponent;

UCLASS()
class SPARTA_HOMEWORK_07_API ADronePawn : public APawn
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DroneMoveMent|Component", Meta = (AllowPrivateAccess = "true"))
	UArrowComponent* ForwardArrowComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DroneMoveMent|Component", Meta = (AllowPrivateAccess = "true"))
	UArrowComponent* RightArrowComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DroneMoveMent|Component", Meta = (AllowPrivateAccess = "true"))
	UArrowComponent* UpArrowComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DroneMoveMent|Component", Meta = (AllowPrivateAccess = "true"))
	UArrowComponent* BackwardArrowComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DroneMoveMent|Component", Meta = (AllowPrivateAccess = "true"))
	UArrowComponent* LeftArrowComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DroneMoveMent|Component", Meta = (AllowPrivateAccess = "true"))
	UArrowComponent* DownArrowComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DroneMoveMent|Property", Meta = (AllowPrivateAccess = "true"))
	FVector CurrentVelocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DroneMoveMent|Property", Meta = (AllowPrivateAccess = "true"))
	float CurrentSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DroneMoveMent|Property", Meta = (AllowPrivateAccess = "true"))
	float CurrentRoll;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DroneMoveMent|Property", Meta = (AllowPrivateAccess = "true"))
	float CurrentPitch;

	FVector MoveInput;
	bool bMove;
	bool bTiltReverse;

	float TargetSpeed;
	float AccelInterp;
	float TargetRoll ;
	float TargetPitch;
	float Gravity;
	float VerticalVelocity; // 현재 Z축 속도
	bool bIsGround;

	bool bDroneOff;
	bool bIsGrabbing;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Drone|Component")
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Drone|Component")
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Drone|Component")
	UPhysicsHandleComponent* PhysicsHandleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera|Component")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera|Component")
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DroneMoveMent|Property")
	float MaxSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DroneMoveMent|Property")
	float Acceleration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DroneMoveMent|Property")
	float Deceleration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DroneMoveMent|Property")
	float RotationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DroneMoveMent|Property")
	float RotationInterpSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DroneMoveMent|Property")
	float MaxTiltAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DroneMoveMent|Property")
	float TiltSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DroneMoveMent|Property")
	float SpeedFactor; // 속도 조절 인자 (값이 클수록 더 빠르게 변화)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DroneMoveMent|Property")
	float AccelerationCurveExponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DroneGrab|Property")
	float GrabLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DroneGrab|Property")
	float HoldDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DroneGrab|Property")
	FVector GrabDirection;
	
public:
	ADronePawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SetEnhancedInput();

	void CheckGround();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void DroneOff(const FInputActionValue& value);
	UFUNCTION()
	void ReturnPlayer(const FInputActionValue& value);
	UFUNCTION()
	void Grab(const FInputActionValue& value);
};
