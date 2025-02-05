#include "LaserSplit.h"
#include "Laser.h"
#include "LaserSpawner.h"
#include "MovieSceneTracksComponentTypes.h"

ALaserSplit::ALaserSplit()
	: SpawnLaserCount(1)
	, SelectableType(SELECTABLE_TYPE::GRAB_HOLDROT)
{
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComp->SetupAttachment(RootComponent);
}

void ALaserSplit::BeginPlay()
{
	Super::BeginPlay();
}

void ALaserSplit::SpawnSplitLaser(ALaser* LaserHit)
{
	float Deg = 360.f / SpawnLaserCount;
	float AccDeg = 0;
	for (int i = 0; i < SpawnLaserCount; i++)
	{
		FVector NewLaserLocation = ArrowComp->GetComponentLocation();
		// 2. 회전 중심(현재 Actor의 위치)을 기준으로 이동
		FVector TranslatedPoint = NewLaserLocation - GetActorLocation();
		// 3. 원점 기준으로 회전
		TranslatedPoint = TranslatedPoint.RotateAngleAxis(AccDeg, FVector::ZAxisVector);
		// 4. 회전 중심 복원
		NewLaserLocation = TranslatedPoint + GetActorLocation();
		// 5. 회전 각도 설정
		FRotator NewLaserRotation = ArrowComp->GetComponentRotation();
		NewLaserRotation.Add(0.f, AccDeg, 0.f);
		// 6. 새로운 Laser 생성
		ALaser* NewLaser = GetWorld()->SpawnActor<ALaser>(BPLaserRef, NewLaserLocation, NewLaserRotation);
		
		NewLaser->SetLaserID(LaserHit->GetLaserID().Append("S"));
		NewLaser->SetLaserColor(LaserHit->GetLaserColor());
		NewLaser->ChangeColor();
		NewLaser->SetLaserSpawnerRef(LaserSpawnerRef);

		NewLaser->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

		LaserAttached.Add(NewLaser);
		LaserSpawnerRef->AddLaserChild(NewLaser);

		AccDeg += Deg;
	}
}

void ALaserSplit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ALaser* ALaserSplit::LaserBounce(ALaser* LaserHit, bool HitResponse, const FHitResult& HitInformation)
{
	if (IsValid(LaserHit))
	{
		LaserSpawnerRef = LaserHit->GetLaserSpawner();

		if (!HitResponse || (!bActive && HitResponse))
		{
			bActive = HitResponse;
			if (bActive)
			{
				SpawnSplitLaser(LaserHit);
				return nullptr;
				// FVector NewLaserLocation = ArrowComp->GetComponentLocation();
				// // 2. 회전 중심(현재 Actor의 위치)을 기준으로 이동
				// FVector TranslatedPoint = NewLaserLocation - GetActorLocation();
				// // 3. 원점 기준으로 회전
				// TranslatedPoint = TranslatedPoint.RotateAngleAxis(45, FVector::ZAxisVector);
				// // 4. 회전 중심 복원
				// NewLaserLocation = TranslatedPoint + GetActorLocation();
				// // 5. 회전 각도 설정
				// FRotator NewLaserRotation = ArrowComp->GetComponentRotation();
				// NewLaserRotation.Add(0.f, 45.f, 0.f);
				// // 6. 새로운 Laser 생성
				// ALaser* NewLaser = GetWorld()->SpawnActor<ALaser>(BPLaserRef, NewLaserLocation, NewLaserRotation);
				//
				//
				// NewLaser->SetLaserID(LaserHit->GetLaserID().Append("S"));
				// NewLaser->SetLaserColor(LaserHit->GetLaserColor());
				// NewLaser->ChangeColor();
				// NewLaser->SetLaserSpawnerRef(LaserSpawnerRef);
				//
				// NewLaser->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
				// //NewLaser->AttachToComponent(StaticMeshComp, FAttachmentComponent::StaticMeshAttachment());
				//
				// LaserAttached.Add(NewLaser);
				// LaserSpawnerRef->AddLaserChild(NewLaser);
				//
				// return NewLaser;
			}
			else
			{
				// LaserSpawnerRef->ClearAllLaserAtteched();
				// return nullptr;
			}
		}
	}
	return nullptr;
}

SELECTABLE_TYPE ALaserSplit::GetSelectableType()
{
	return SelectableType;
}

void ALaserSplit::Select()
{
}
