#include "Laser.h"

#include "LaserReflection.h"
#include "LaserSpawner.h"

ALaser::ALaser()
	: LastHitActor(nullptr)
	, CurrentHitActor(nullptr)
	, BounceLaser(nullptr)
{
	//LaserID = GetName();
}

void ALaser::BeginPlay()
{
	Super::BeginPlay();

	ChangeColor();
}

void ALaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetLaserEndPoint();

	if (IsValid(CurrentHitActor))
	{
		if (LastHitActor == CurrentHitActor)
		{
			ALaserReflection* pLaserReflection = Cast<ALaserReflection>(LastHitActor);
			if (pLaserReflection)
			{
				FVector NewLocation = HitResult.ImpactPoint;
				BounceLaser->SetActorLocation(NewLocation);

				FRotator NewRotation = pLaserReflection->CalRotation(HitResult.ImpactPoint, HitResult.ImpactNormal, HitResult.TraceStart);
				BounceLaser->SetActorRotation(NewRotation);
			}
		}
		else
		{
			if (Cast<ILaserInterface>(LastHitActor))
			{
				LaserSpawnerRef->RemoveFollowingLaser(this);
			}
			if (Cast<ILaserInterface>(CurrentHitActor))
			{
				BounceLaser = Cast<ILaserInterface>(CurrentHitActor)->LaserBounce(this, true, HitResult);
			}
		}


		LastHitActor = CurrentHitActor;
	}
	else
	{
		if (Cast<ILaserInterface>(LastHitActor))
		{
			BounceLaser = Cast<ILaserInterface>(LastHitActor)->LaserBounce(this, true, HitResult);
		}
		
		LastHitActor = CurrentHitActor;
	}
}

void ALaser::ChangeColor() const
{
	Super::StaticMeshComp->SetColorParameterValueOnMaterials(TEXT("Color"), LaserColor);
}

void ALaser::SetLaserEndPoint()
{
	FVector UpVector = GetActorForwardVector();
	FVector Location = GetActorLocation();

	float StartOffset = 3.f;
	float LineRange = 5000.f;
	FVector StartPoint = Location + (UpVector * StartOffset);
	FVector EndPoint = StartPoint + (UpVector * LineRange);
	
	FCollisionQueryParams CollisionQueryParamsParam(NAME_None, false, this);
	
	bool IsHitResult = GetWorld()->LineTraceSingleByChannel(HitResult, StartPoint, EndPoint, ECC_Visibility, CollisionQueryParamsParam);

	float ScaleX = 0.f;
	float MeshBoundExtent_X = StaticMeshComp->GetStaticMesh()->GetBounds().BoxExtent.X;
	if (IsHitResult)
	{
		CurrentHitActor = HitResult.GetActor();
		ScaleX = (HitResult.Distance + 1.f) / (MeshBoundExtent_X * 2.f);
	}
	else
	{
		ScaleX = LineRange / (MeshBoundExtent_X * 2.f);
	}
	StaticMeshComp->SetRelativeScale3D(FVector(ScaleX, 0.05f, 0.05f));
}

FString ALaser::GetLaserID() const
{
	return LaserID;
}

void ALaser::SetLaserID(const FString& ID)
{
	LaserID = ID;
}

bool ALaser::CancelTargetHit(bool IsOverride)
{
	if (IsValid(LastHitActor))
	{
		if (Cast<ALaser_Parents>(LastHitActor))
		{
			if (Cast<ILaserInterface>(LastHitActor)
			&& LaserAttached.IsEmpty())
			{
				// 원래 BP에서는 3번째 인자 없었음.
				Cast<ILaserInterface>(LastHitActor)->LaserBounce(this, false, HitResult);
			}
			else
			{
				Cast<ALaser_Parents>(LastHitActor)->SetbActive(false);
			}
		}
	}
	if (IsOverride)
	{
		return false;
	}
	else
	{
		Destroy();
		return true;
	}
}

void ALaser::UpdateLaserID(const FString& strAppend)
{
	LaserID = LaserID.Append(strAppend);
}