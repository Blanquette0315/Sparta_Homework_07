#include "ActionButton.h"

#include "DronePawn.h"
#include "MoveDoor.h"
#include "PawnCharacter.h"
#include "SpartaCharacter.h"
#include "Components/BoxComponent.h"

AActionButton::AActionButton()
	: ButtonType(EActionButtonType::None)
	, IsSpawned(false)
	, IsMoved(false)
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	SetRootComponent(StaticMeshComp);

	BTNStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BTNStaticMeshComp"));
	BTNStaticMeshComp->SetupAttachment(StaticMeshComp);
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(BTNStaticMeshComp);

}

void AActionButton::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AActionButton::OnComponentBeginOverlap);
}

void AActionButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActionButton::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
											UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ButtonType == EActionButtonType::Spawn)
	{
		if (!IsSpawned)
		{
			if (Cast<ADronePawn>(OtherActor)
				|| Cast<ASpartaCharacter>(OtherActor)
				|| Cast<APawnCharacter>(OtherActor))
			{
				GetWorld()->SpawnActor<AActor>(BPSpawnRef, SpawnLocation, SpawnRotation);
		
				BTNStaticMeshComp->SetColorParameterValueOnMaterials(TEXT("GlowColor"), FColor::Red);

				if (SpawnOnce)
				{
					IsSpawned = true;
				}
			}
		}
	}

	else if (ButtonType == EActionButtonType::MoveActor)
	{
		if (!IsMoved)
		{
			if (Cast<ADronePawn>(OtherActor)
				|| Cast<ASpartaCharacter>(OtherActor)
				|| Cast<APawnCharacter>(OtherActor))
			{
				Cast<AMoveDoor>(WorldMoveRef)->SetDoorActive();
				BTNStaticMeshComp->SetColorParameterValueOnMaterials(TEXT("GlowColor"), FColor::Red);
				IsMoved = true;
			}
		}
	}
}

