#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovePlatform.generated.h"

UCLASS()
class SPARTA_HOMEWORK_07_API AMovePlatform : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY()
	FVector OriginLocation;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MovePlatform|Component")
	USceneComponent* SceneRootComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MovePlatform|Component")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MovePlatform|Properties")
	float Distance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MovePlatform|Properties")
	float MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MovePlatform|Properties")
	FVector Direction;
	
public:	
	AMovePlatform();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void MovePlatform(float DeltaTime);

};