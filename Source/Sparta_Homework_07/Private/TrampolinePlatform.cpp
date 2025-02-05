#include "TrampolinePlatform.h"
//#include "Sparta_Homework_07/Sparta_Homework_07Character.h"

ATrampolinePlatform::ATrampolinePlatform()
	: Force(0.f)
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComp);
}

void ATrampolinePlatform::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComp->OnComponentHit.AddDynamic(this, &ATrampolinePlatform::OnPlatformHit);
}

void ATrampolinePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrampolinePlatform::OnPlatformHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//if (Cast<ASparta_Homework_06Character>(OtherActor))
	//{
	//	Cast<ASparta_Homework_06Character>(OtherActor)->LaunchCharacter(Hit.ImpactNormal * -1.f * Force, false, false);
	//}
}