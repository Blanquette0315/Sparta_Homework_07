#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActionButton.generated.h"

class UBoxComponent;

UENUM(BlueprintType)
enum class EActionButtonType : uint8
{
	None UMETA(DisplayName = "None"),
	Spawn UMETA(DisplayName = "Spawn"),
	MoveActor UMETA(DisplayName = "MoveActor"),

	End
};

UCLASS()
class SPARTA_HOMEWORK_07_API AActionButton : public AActor
{
	GENERATED_BODY()

private:
	bool IsSpawned;
	bool IsMoved;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Btn|Component")
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Btn|Component")
	UStaticMeshComponent* BTNStaticMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Btn|Component")
	UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Btn|Component")
	EActionButtonType ButtonType;

	// MoveActor 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Btn|MoveActorType|Properties")
	AActor* WorldMoveRef;
	
	// Spawn 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Btn|SpawnType|Properties")
	TSubclassOf<AActor> BPSpawnRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Btn|SpawnType|Properties")
	FVector SpawnLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Btn|SpawnType|Properties")
	FRotator SpawnRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Btn|SpawnType|Properties")
	bool SpawnOnce;
	
public:	
	AActionButton();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Btn|Functions")
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
