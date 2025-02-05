#include "PawnCharacter.h"

#include "SpartaCharacter.h"
#include "DronePawn.h"
#include "SpartaPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

APawnCharacter::APawnCharacter()
	: bMove(false)
	, bPrevMove(false)
	, NormalSpeed(600.0f)
	, SprintSpeedMultiplier(1.70f)
	, AirSpeedMultiplier(0.50f)
	, bIsGround(false)
	, bPressedJump(false)
	, JumpSpeed(0.0f)
	, JumpForce(500.0f)
	, CurrentSpeed(0.0f)
	, Gravity(980.0f)
	, PreviousLocation(FVector::ZeroVector)
	, TotalVelocity(FVector::ZeroVector)
	, bIsFalling(false)
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(CapsuleComp);
	CapsuleComp->SetSimulatePhysics(false);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SkeletalMeshComp->SetupAttachment(RootComponent);
	SkeletalMeshComp->SetSimulatePhysics(false);
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->TargetArmLength = 300;
	SpringArmComp->bUsePawnControlRotation = false;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
	MaxWalkSpeed = NormalSpeed;
}

void APawnCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void APawnCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bPrevMove = false;


	if (bMove || !bIsGround)
	{
		FVector NewLocation = CalNewLocation(DeltaTime);

		// 충돌 검사
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this); // 자기 자신은 충돌 검사에서 제외
		FVector StartPoint = GetActorLocation();
		StartPoint.Z -= (CapsuleComp->GetUnscaledCapsuleHalfHeight() * 0.5f) + 15.0f;

		//DrawDebugSphere(GetWorld(), StartPoint, 15, 26, FColor::Red);
		
		bool bHit = GetWorld()->SweepSingleByChannel(
			HitResult,             // 충돌 결과 저장
			StartPoint,				// 시작 위치
			NewLocation,             // 목표 위치
			FQuat::Identity,         // 회전 없음
			ECC_Pawn,                // 충돌 채널 (Pawn으로 설정)
			FCollisionShape::MakeSphere(30.0f), // 충돌 체크 모양 (구)
			Params
		);

		if (!bHit) 
		{
			SetActorLocation(NewLocation); // 충돌이 없으면 이동
		}
		else 
		{
			// 슬라이딩 처리: 벽의 법선(Normal)과 이동 방향을 사용하여 새로운 이동 벡터 계산
			FVector Direction = (GetActorLocation() - NewLocation) * -1;
			Direction = Direction.GetSafeNormal();
			FVector SlideDirection = FVector::VectorPlaneProject(Direction, HitResult.Normal);
			FVector SlideLocation = GetActorLocation() + (SlideDirection * MaxWalkSpeed * DeltaTime);

			// 슬라이딩 방향으로 재충돌 검사
			bool bSlideHit = GetWorld()->SweepSingleByChannel(
				HitResult,
				StartPoint,
				SlideLocation,
				FQuat::Identity,
				ECC_Pawn,
				FCollisionShape::MakeSphere(30.0f),
				Params
			);

			// 해결책 1: 벽에 완전히 정면으로 부딪힌 경우는 이동하지 않음
			if (FVector::DotProduct(Direction, HitResult.Normal) < -0.9f) {}
			else
			{
				if (!bSlideHit) 
				{
					// 슬라이딩 방향으로 이동 허용
					SetActorLocation(SlideLocation);
				}
				else 
				{
					UE_LOG(LogTemp, Warning, TEXT("벽에 완전히 막혀 이동 불가!"));
					FVector SmallOffset = HitResult.Normal * 2.0f;
					SetActorLocation(GetActorLocation() + SmallOffset);
				}
			}
		}
	}
	
	FVector CurrentLocation = GetActorLocation();
	float FrameDistance = FVector::Dist(PreviousLocation, CurrentLocation); 
	CurrentSpeed = FrameDistance / DeltaTime; // 실제 이동 속도
	
	TotalVelocity = (CurrentLocation - PreviousLocation) / DeltaTime;
	PreviousLocation = CurrentLocation;

	CheckGround();
}

FVector APawnCharacter::CalNewLocation(float DeltaTime)
{
	FVector NewLocation = GetActorLocation();
	TotalVelocity = FVector::ZeroVector;
	
	if (bMove)
	{
		float Speed = MaxWalkSpeed * DeltaTime;
		if (!bIsGround && bJumpFirst)
		{
			Speed *= AirSpeedMultiplier;
		}
		ControlInputVector = ControlInputVector.GetSafeNormal();		
		//TotalVelocity += (ControlInputVector * Speed);
		NewLocation += (Internal_ConsumeMovementInputVector() * Speed);

		bMove = false;
		bPrevMove = true;
	}

	if (!bIsGround)
	{
		JumpSpeed -= Gravity * DeltaTime;
		NewLocation += (FVector::UpVector * JumpSpeed * DeltaTime);
	}
	
	return NewLocation;
}

void APawnCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput
		= Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ASpartaPlayerController* PlayerController
			= Cast<ASpartaPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(PlayerController->MoveAction
					, ETriggerEvent::Triggered
					,this
					, &APawnCharacter::Move);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(PlayerController->JumpAction
					, ETriggerEvent::Triggered
					,this
					, &APawnCharacter::StartJump);

				EnhancedInput->BindAction(PlayerController->JumpAction
					, ETriggerEvent::Completed
					,this
					, &APawnCharacter::StopJump);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(PlayerController->LookAction
					, ETriggerEvent::Triggered
					,this
					, &APawnCharacter::Look);
			}
			
			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(PlayerController->SprintAction
					, ETriggerEvent::Triggered
					,this
					, &APawnCharacter::StartSprint);

				EnhancedInput->BindAction(PlayerController->SprintAction
					, ETriggerEvent::Completed
					,this
					, &APawnCharacter::StopSprint);
			}

			if (PlayerController->SpawnDroneAction)
			{
				EnhancedInput->BindAction(PlayerController->SpawnDroneAction
					, ETriggerEvent::Triggered
					,this
					, &APawnCharacter::SpawnDrone);
			}
			
			if (PlayerController->ChangePlayerAction)
			{
				EnhancedInput->BindAction(PlayerController->ChangePlayerAction
					, ETriggerEvent::Triggered
					,this
					, &APawnCharacter::ChangePlayer);
			}
		}
	}
}

void APawnCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;
	
	const FVector2d MoveInput = value.Get<FVector2d>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}

	if (!bIsGround && !bPrevMove && !bJumpFirst)
	{
		bJumpFirst = true;
	}
	
	bMove = true;
}

void APawnCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>() && bIsGround)
	{
		bPressedJump = true;
		JumpSpeed = JumpForce;
		bIsGround = false;

		if (bMove)
			bJumpFirst = false;
		else
			bJumpFirst = true;
	}
}

void APawnCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		bPressedJump = false;
	}
}

void APawnCharacter::Look(const FInputActionValue& value)
{
	if (!Controller) return;

	FVector2d LookInput = value.Get<FVector2d>();
	
	// 액터 회전 (Yaw 적용)
	FRotator ActorRotation = GetActorRotation();
	ActorRotation.Yaw += LookInput.X;
	SetActorRotation(ActorRotation);

	// SpringArm 회전 (Pitch 적용)
	FRotator SpringArmRotation = SpringArmComp->GetRelativeRotation();
	SpringArmRotation.Pitch -= LookInput.Y;

	// Pitch 각도 제한 (-89 ~ 89도)
	SpringArmRotation.Pitch = FMath::Clamp(SpringArmRotation.Pitch, -89.0f, 89.0f);

	// 최종 회전 적용
	SpringArmComp->SetRelativeRotation(SpringArmRotation);
	
}

void APawnCharacter::StartSprint(const FInputActionValue& value)
{
	MaxWalkSpeed = SprintSpeed;
}

void APawnCharacter::StopSprint(const FInputActionValue& value)
{
	MaxWalkSpeed = NormalSpeed;
}

void APawnCharacter::SpawnDrone(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Cast<ASpartaPlayerController>(GetController())->SetPlayerPawn(this);
		FVector NewLocation = GetActorLocation();
		NewLocation += GetActorForwardVector() * 100.0f;
		ADronePawn* newDrone = GetWorld()->SpawnActor<ADronePawn>(BPDroneRef, NewLocation, FRotator::ZeroRotator);
		Cast<ASpartaPlayerController>(GetController())->ChangeMappingContext(1);
		Cast<ASpartaPlayerController>(GetController())->ChangePossess(newDrone);
	}
}

void APawnCharacter::ChangePlayer(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		FVector NewLocation = GetActorLocation();
		ASpartaCharacter* newPlayer = GetWorld()->SpawnActor<ASpartaCharacter>(BPSpartaRef, NewLocation, GetActorRotation());
		Cast<ASpartaPlayerController>(GetController())->SetPlayerPawn(newPlayer);
		Cast<ASpartaPlayerController>(GetController())->ChangeMappingContext(0);
		Cast<ASpartaPlayerController>(GetController())->ChangePossess(newPlayer);

		Destroy();
	}
}

void APawnCharacter::CheckGround()
{
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0, 0, 95);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false);
	if (GetWorld()->SweepSingleByChannel(HitResult,Start,End,FQuat::Identity,ECC_Pawn,FCollisionShape::MakeSphere(15.0f),	Params))
	{
		Landed();
	}
	else
	{
		if (TotalVelocity.GetSafeNormal().Z < 0.0f)
		{
			bIsFalling = true;
		}
		
		bIsGround = false;
	}
}

// 바닥 감지 시 착지 처리
void APawnCharacter::Landed()
{
	bIsFalling = false;
	bIsGround = true;
	JumpSpeed = 0.f;
}
