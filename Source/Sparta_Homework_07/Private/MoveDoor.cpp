#include "MoveDoor.h"

AMoveDoor::AMoveDoor()
	: Direction(FVector::ZeroVector)
	, MoveSpeed(0.0f)
	, TargetDistance(0.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	SetRootComponent(StaticMeshComp);
}

void AMoveDoor::BeginPlay()
{
	Super::BeginPlay();

	OriginLocation = GetActorLocation();
}

void AMoveDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bActive)
	{
		if (FVector::Dist(OriginLocation, GetActorLocation()) < TargetDistance)
		{
			FVector NewLocation = GetActorLocation();
			NewLocation += Direction * MoveSpeed * DeltaTime;

			SetActorLocation(NewLocation);
		}
	}
}

