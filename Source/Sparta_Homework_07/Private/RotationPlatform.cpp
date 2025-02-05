#include "RotationPlatform.h"

ARotationPlatform::ARotationPlatform()
	: RotationSpeed(0.f)
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComp->SetupAttachment(SceneRoot);
}

void ARotationPlatform::BeginPlay()
{
	Super::BeginPlay();
}

void ARotationPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!RotationSpeed.IsZero())
	{
		AddActorLocalRotation(RotationSpeed * DeltaTime);
	}
}

