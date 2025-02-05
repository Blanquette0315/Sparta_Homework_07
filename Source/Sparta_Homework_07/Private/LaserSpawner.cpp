#include "LaserSpawner.h"
#include "Laser.h"
#include "Kismet/KismetSystemLibrary.h"

ALaserSpawner::ALaserSpawner()
	: SpawnedLaser(nullptr)
{
	SpawnLaserArrow = CreateDefaultSubobject<UArrowComponent>("Arrow");
	SpawnLaserArrow->SetupAttachment(StaticMeshComp);
}

void ALaserSpawner::BeginPlay()
{
	Super::BeginPlay();

	LaserSpawnerRef = this;
	FVector SpawnLocation = SpawnLaserArrow->GetComponentLocation();
	FRotator Rotation = GetActorRotation();
	Rotation = SpawnLaserArrow->GetRelativeRotation() * -1.f;
	
	SpawnedLaser = GetWorld()->SpawnActor<ALaser>(BPLaserRef, SpawnLocation, Rotation);
	SpawnedLaser->SetLaserColor(LaserColor);
	SpawnedLaser->ChangeColor();
	SpawnedLaser->SetLaserSpawnerRef(this);
	SpawnedLaser->SetLaserID(UKismetSystemLibrary::GetDisplayName(this));

	ChildLasers.Add(SpawnedLaser);
}

void ALaserSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALaserSpawner::RemoveFollowingLaser(ALaser* LaserRef)
{
	for (int i = ChildLasers.Num() - 1; i >= 0; --i)
	{
		if (IsValid(ChildLasers[i]) && ChildLasers[i]->GetLaserID().Contains(LaserRef->GetLaserID()))
		{
			if (ChildLasers[i]->CancelTargetHit(ChildLasers[i]->GetLaserID() == LaserRef->GetLaserID()))
			{
				ChildLasers.RemoveAt(i);
			}
		}
	}
}

void ALaserSpawner::ClearAllLaserAtteched()
{
	for (int i = 0; i < ChildLasers.Num(); ++i)
	{
		if (IsValid(ChildLasers[i]))
		{
			ChildLasers[i]->Destroy();
		}
	}

	ChildLasers.Empty();
}

void ALaserSpawner::AddLaserChild(ALaser* LaserRef)
{
	ChildLasers.Add(LaserRef);
}
