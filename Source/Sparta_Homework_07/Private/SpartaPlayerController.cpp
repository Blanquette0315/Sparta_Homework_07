#include "SpartaPlayerController.h"

#include "DronePawn.h"
#include "EnhancedInputSubsystems.h"

ASpartaPlayerController::ASpartaPlayerController()
	: PlayerPawn(nullptr)
	, InputMappingContext(nullptr)
	, MoveAction(nullptr)
	, JumpAction(nullptr)
	, LookAction(nullptr)
	, SprintAction(nullptr)
	, SpawnDroneAction(nullptr)
	, DronInputMappingContext(nullptr)
	, DronMoveAction(nullptr)
	, DronLookAction(nullptr)
{
	
}

void ASpartaPlayerController::ChangePossess(APawn* NewPawn)
{
	if (nullptr != NewPawn)
	{
		PlayerPawn = GetPawn();
		UnPossess();  // 기존 Pawn에서 빙의 해제
		Possess(NewPawn); // 새로운 Pawn에 빙의

		if (ADronePawn* DronePawn = Cast<ADronePawn>(NewPawn))
		{
			DronePawn->SetEnhancedInput();
		}
	}
}

void ASpartaPlayerController::ChangeMappingContext(int Type)
{
	 if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	 {
	 	if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
	 		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	 	{
	 		if (nullptr != InputMappingContext)
	 		{
	 			SubSystem->ClearAllMappings();
	 			// Character
	 			if (Type == 0)
	 			{
	 				// 0은 가장 높은 우선순위
	 				// 다른 IMC들과 겹치는 특수한 상황이 있다면 이 우선순위를 체크해 우선순위가 높은 것으로 할당한다.
	 				SubSystem->AddMappingContext(InputMappingContext, 0);
	 			}
	 			// Drone
	 			else if (Type == 1)
	 			{
	 				SubSystem->AddMappingContext(DronInputMappingContext, 1);
	 			}
	 		}
	 	}
	 }
}

void ASpartaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (nullptr != InputMappingContext)
			{
				// 0은 가장 높은 우선순위
				// 다른 IMC들과 겹치는 특수한 상황이 있다면 이 우선순위를 체크해 우선순위가 높은 것으로 할당한다.
				SubSystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}
