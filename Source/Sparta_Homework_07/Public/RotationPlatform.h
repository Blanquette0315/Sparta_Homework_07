#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotationPlatform.generated.h"

UCLASS()
class SPARTA_HOMEWORK_07_API ARotationPlatform : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MovePlatform|Components")
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="MovePlatform|Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MovePlatform|Properties")
	FRotator RotationSpeed;
	
public:	
	ARotationPlatform();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
