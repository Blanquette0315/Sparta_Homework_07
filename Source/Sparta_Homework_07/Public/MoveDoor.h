#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoveDoor.generated.h"

UCLASS()
class SPARTA_HOMEWORK_07_API AMoveDoor : public AActor
{
	GENERATED_BODY()

private:
	bool bActive;
	FVector OriginLocation;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door|Component")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Door|Property")
	FVector Direction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Door|Property")
	float MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Door|Property")
	float TargetDistance;
	
public:	
	AMoveDoor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SetDoorActive() { bActive = true; }
};
