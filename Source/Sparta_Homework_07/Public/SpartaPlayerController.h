#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpartaPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class SPARTA_HOMEWORK_07_API ASpartaPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	APawn* PlayerPawn;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|CharacterPawn")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|CharacterPawn")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|CharacterPawn")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|CharacterPawn")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|CharacterPawn")
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|CharacterPawn")
	UInputAction* SpawnDroneAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|CharacterPawn")
	UInputAction* ChangePlayerAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|CharacterPawn")
	UInputAction* GrabAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|CharacterPawn")
	UInputAction* SelectRotAction;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|DronePawn")
	UInputMappingContext* DronInputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|DronePawn")
	UInputAction* DronMoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|DronePawn")
	UInputAction* DronLookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|DronePawn")
	UInputAction* DroneOffAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|DronePawn")
	UInputAction* DroneReturnPlayerAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|DronePawn")
	UInputAction* DroneGrabAction;
	
protected:
	virtual void BeginPlay() override;
	
public:
	ASpartaPlayerController();

	void SetPlayerPawn(APawn* PlayerCharacterPawn) {PlayerPawn = PlayerCharacterPawn;}
	APawn* GetPlayerPawn() const {return PlayerPawn;}
	void ChangePossess(APawn* NewPawn);
	void ChangeMappingContext(int Type);
};
