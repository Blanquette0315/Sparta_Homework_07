#include "SpartaCharacter.h"

#include "DronePawn.h"
#include "SpartaPlayerController.h"
#include "EnhancedInputComponent.h"
#include "SelectableInterface.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

ASpartaCharacter::ASpartaCharacter()
	: NormalSpeed(600.f)
	, SprintSpeedMultiplier(1.7f)
	, GrabLength(100.0f)
	, HoldDistance(100.0f)
	, bIsGrabbing(false)
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300;
	SpringArmComp->bUsePawnControlRotation = true;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	PhysicsHandleComp = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}

void ASpartaCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpartaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (bIsGrabbing)
	{
		FVector HoldLocation = GetActorLocation() + GetActorForwardVector() * HoldDistance;
		if (Cast<ISelectableInterface>(PhysicsHandleComp->GetGrabbedComponent()->GetOwner())->GetSelectableType() == SELECTABLE_TYPE::GRAB_HOLDROT)
		{
			PhysicsHandleComp->SetTargetLocationAndRotation(HoldLocation, PhysicsHandleComp->GetGrabbedComponent()->GetOwner()->GetActorRotation());
		}
		else
		{
			PhysicsHandleComp->SetTargetLocationAndRotation(HoldLocation, GetActorRotation());
		}
		//PhysicsHandleComp->SetTargetLocationAndRotation(HoldLocation, GetActorRotation());
	}
}

void ASpartaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
					, &ASpartaCharacter::Move);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(PlayerController->JumpAction
					, ETriggerEvent::Triggered
					,this
					, &ASpartaCharacter::StartJump);

				EnhancedInput->BindAction(PlayerController->JumpAction
					, ETriggerEvent::Completed
					,this
					, &ASpartaCharacter::StopJump);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(PlayerController->LookAction
					, ETriggerEvent::Triggered
					,this
					, &ASpartaCharacter::Look);
			}
			
			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(PlayerController->SprintAction
					, ETriggerEvent::Triggered
					,this
					, &ASpartaCharacter::StartSprint);

				EnhancedInput->BindAction(PlayerController->SprintAction
					, ETriggerEvent::Completed
					,this
					, &ASpartaCharacter::StopSprint);
			}
			
			if (PlayerController->SpawnDroneAction)
			{
				EnhancedInput->BindAction(PlayerController->SpawnDroneAction
					, ETriggerEvent::Triggered
					,this
					, &ASpartaCharacter::SpawnDrone);
			}

			if (PlayerController->GrabAction)
			{
				EnhancedInput->BindAction(PlayerController->GrabAction
					, ETriggerEvent::Triggered
					,this
					, &ASpartaCharacter::Grab);
			}

			if (PlayerController->SelectRotAction)
			{
				EnhancedInput->BindAction(PlayerController->SelectRotAction
					, ETriggerEvent::Triggered
					,this
					, &ASpartaCharacter::SelectRot);
			}
		}
	}
}

void ASpartaCharacter::Move(const FInputActionValue& value)
{
	// 2DVector로 받기로 했다.
	if (!Controller) return;

	const FVector2d MoveInput = value.Get<FVector2d>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		// 캐릭터 무브먼트 컴포넌트의 함수
		// 캐릭터의 정면 벡터 방향으로 X만큼 이동시켜라.
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void ASpartaCharacter::StartJump(const FInputActionValue& value)
{
	//if (!Controller) return;
	// 컨트롤러 체크가 필요없는 이유는 Jump()내부에서 한번 체크를 해주기도 하고,
	// 무엇보다 Move는 Controller가 있어야 호출할 수 있는 함수를 사용했는데,
	// 이번엔 사용하지 않기 때문에 체크할 필요가 없다.
	
	if (value.Get<bool>())
	{
		// 캐릭터 무브먼트에서 제공해주는 점프 함수이다.
		Jump();
	}
}

void ASpartaCharacter::StopJump(const FInputActionValue& value)
{	
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void ASpartaCharacter::Look(const FInputActionValue& value)
{
	FVector2d LookInput = value.Get<FVector2d>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void ASpartaCharacter::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ASpartaCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

void ASpartaCharacter::SpawnDrone(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Cast<ASpartaPlayerController>(GetController())->SetPlayerPawn(this);
		FVector NewLocation = GetActorLocation();
		NewLocation += GetActorForwardVector() * 200.0f;
		ADronePawn* newDrone = GetWorld()->SpawnActor<ADronePawn>(BPDroneRef, NewLocation, FRotator::ZeroRotator);
		if (IsValid(newDrone))
		{
			Cast<ASpartaPlayerController>(GetController())->ChangeMappingContext(1);
			Cast<ASpartaPlayerController>(GetController())->ChangePossess(newDrone);
		}
	}
}

void ASpartaCharacter::Grab(const FInputActionValue& value)
{
	static bool ToggleGrabbing = false;
	if (value.Get<bool>())
	{
		if (!ToggleGrabbing)
		{
			FVector Start = GetActorLocation();
			FVector End = Start + GetActorForwardVector() * GrabLength;

			FHitResult HitResult;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);

			//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false);
			if (GetWorld()->SweepSingleByChannel(HitResult,Start,End,FQuat::Identity,ECC_Pawn,FCollisionShape::MakeCapsule(35.0f, 47.0f),Params))
			{
				if (Cast<ISelectableInterface>(HitResult.GetActor()))
				{
					SELECTABLE_TYPE TargetType = Cast<ISelectableInterface>(HitResult.GetActor())->GetSelectableType();
					if ( TargetType == SELECTABLE_TYPE::GRAB)
					{
						PhysicsHandleComp->GrabComponentAtLocationWithRotation(HitResult.GetComponent(), NAME_None, HitResult.ImpactPoint, GetActorRotation());
						bIsGrabbing = true;
						ToggleGrabbing = true;
					}
					else if (TargetType == SELECTABLE_TYPE::GRAB_HOLDROT)
					{
						PhysicsHandleComp->GrabComponentAtLocationWithRotation(HitResult.GetComponent(), NAME_None, HitResult.ImpactPoint, HitResult.GetActor()->GetActorRotation());
						bIsGrabbing = true;
						ToggleGrabbing = true;
					}
				}
			}
		}
		else
		{
			PhysicsHandleComp->ReleaseComponent();
			ToggleGrabbing = false;
			bIsGrabbing = false;
		}
	}
}

void ASpartaCharacter::SelectRot(const FInputActionValue& value)
{
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * GrabLength;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false);
	if (GetWorld()->SweepSingleByChannel(HitResult,Start,End,FQuat::Identity,ECC_Pawn,FCollisionShape::MakeCapsule(35.0f, 47.0f),Params))
	{
		if (Cast<ISelectableInterface>(HitResult.GetActor()))
		{
			if (Cast<ISelectableInterface>(HitResult.GetActor())->GetSelectableType() == SELECTABLE_TYPE::ROTATE)
			{
				if (value.Get<float>() > 0)
				{
					Cast<ISelectableInterface>(HitResult.GetActor())->SetRotationSpeed(FRotator(0.0f, 25.0f, 0.0f));
				}
				else if (value.Get<float>() < 0)
				{
					Cast<ISelectableInterface>(HitResult.GetActor())->SetRotationSpeed(FRotator(0.0f, -25.0f, 0.0f));
				}
				Cast<ISelectableInterface>(HitResult.GetActor())->Select();
			}
		}
	}
}
