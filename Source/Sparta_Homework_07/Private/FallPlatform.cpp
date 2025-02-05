#include "FallPlatform.h"

#include "SpartaCharacter.h"
//#include "Sparta_Homework_07/Sparta_Homework_07Character.h"

AFallPlatform::AFallPlatform()
	: IsFalling(false)
	, HoldingTime(0.f)
	, ResetTime(0.f)
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshRoot"));
	SetRootComponent(StaticMeshComp);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComp->SetupAttachment(StaticMeshComp);
}

void AFallPlatform::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AFallPlatform::OnComponentBeginOverlap);

	OriginTransform = GetTransform();
}

void AFallPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFallPlatform::FallingPlatform()
{
	StaticMeshComp->SetSimulatePhysics(true);
	GetWorld()->GetTimerManager().ClearTimer(FallingTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(FallingTimerHandle, this, &AFallPlatform::ResetPlatform, ResetTime, false);
}

void AFallPlatform::ResetPlatform()
{
	StaticMeshComp->SetSimulatePhysics(false);
	GetWorld()->GetTimerManager().ClearTimer(FallingTimerHandle);
	SetActorTransform(OriginTransform);
	IsFalling = false;
}

void AFallPlatform::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
 {
 	if (Cast<ASpartaCharacter>(OtherActor))
 	{
 		if (!IsFalling)
 		{
 			GetWorld()->GetTimerManager().SetTimer(FallingTimerHandle, this, &AFallPlatform::FallingPlatform, HoldingTime, false);
 			IsFalling = true;
 		}
 	}
 }
