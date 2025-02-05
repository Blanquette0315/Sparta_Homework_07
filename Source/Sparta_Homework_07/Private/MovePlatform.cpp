#include "MovePlatform.h"

AMovePlatform::AMovePlatform()
	: OriginLocation(FVector::ZeroVector)
	, Distance(0.f)
	, MoveSpeed(0.f)
	, Direction(FVector::ZeroVector)
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRootComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRootComp);
}

void AMovePlatform::BeginPlay()
{
	Super::BeginPlay();

	OriginLocation = GetActorLocation();
}

void AMovePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Distance > 0.f && !Direction.IsZero())
	{
		MovePlatform(DeltaTime);
	}
}

void AMovePlatform::MovePlatform(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = CurrentLocation + Direction.GetSafeNormal() * MoveSpeed * DeltaTime;
	
	if (FVector::Dist(OriginLocation, NewLocation) >= Distance)
	{
		NewLocation = OriginLocation + Direction.GetSafeNormal() * Distance;
		Direction *= -1;
	}

	SetActorLocation(NewLocation);
}

