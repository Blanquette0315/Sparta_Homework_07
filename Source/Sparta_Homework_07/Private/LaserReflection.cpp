#include "LaserReflection.h"

#include "Laser.h"
#include "LaserSpawner.h"
#include "Kismet/KismetMathLibrary.h"

ALaserReflection::ALaserReflection()
	: SelectableType(SELECTABLE_TYPE::ROTATE)
{
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComp->SetupAttachment(StaticMeshComp);
}

void ALaserReflection::BeginPlay()
{
	Super::BeginPlay();
}

void ALaserReflection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FRotator ALaserReflection::CalRotation(const FVector& ImpactPoint, const FVector& ImpactNormal, const FVector& TraceStart) const
{
	FVector vector = FVector(ImpactPoint.X - TraceStart.X, ImpactPoint.Y - TraceStart.Y, 0.0f).GetSafeNormal();
	FVector normal = FVector(ImpactNormal.X,ImpactNormal.Y, 0.0f).GetSafeNormal();

	FVector mirror = vector.MirrorByVector(normal);
	
	return UKismetMathLibrary::MakeRotFromX(mirror);
}

void ALaserReflection::ClearAttachedLasers(const FString& ClearLaserID)
{
	for (int i = LaserAttached.Num() - 1; i >= 0; i--)
	{
		if (IsValid(LaserAttached[i])
			&& LaserAttached[i]->GetLaserID().Contains(ClearLaserID))
		{
			LaserAttached[i]->Destroy();
			LaserAttached.RemoveAt(i);
		}
	}
}

ALaser* ALaserReflection::LaserBounce(ALaser* LaserHit, bool HitResponse, const FHitResult& HitInformation)
{
	if (HitResponse)
	{
		FRotator NewRotation = CalRotation(HitInformation.ImpactPoint, HitInformation.ImpactNormal, HitInformation.TraceStart);
		//FVector UnitDirection = FVector(HitInformation.TraceEnd - HitInformation.TraceStart).GetSafeNormal();
		//FMath::GetReflectionVector(UnitDirection, ArrowComp->GetForwardVector().GetSafeNormal());

		ALaser* SpawnedLaser = GetWorld()->SpawnActor<ALaser>(BPLaserRef, HitInformation.ImpactPoint, NewRotation);
		SpawnedLaser->SetLaserID(LaserHit->GetLaserID().Append(FString("B")));
		SpawnedLaser->SetLaserColor(LaserHit->GetLaserColor());
		SpawnedLaser->ChangeColor();
		SpawnedLaser->SetLaserSpawnerRef(LaserHit->GetLaserSpawner());

		LaserAttached.Add(SpawnedLaser);
		LaserHit->GetLaserSpawner()->AddLaserChild(SpawnedLaser);

		return SpawnedLaser;
	}
	else
	{
		ClearAttachedLasers(LaserHit->GetLaserID());
		return nullptr;
	}
}

SELECTABLE_TYPE ALaserReflection::GetSelectableType()
{
	return SelectableType;
}

void ALaserReflection::Select()
{
	AddActorLocalRotation(SelectRotationSpeed * GetWorld()->GetDeltaSeconds());
}

void ALaserReflection::SetRotationSpeed(FRotator RotationSpeed)
{
	SelectRotationSpeed = RotationSpeed;
}
