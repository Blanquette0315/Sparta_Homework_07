#include "Laser_Parents.h"
#include "Laser.h"
#include "LaserSpawner.h"

ALaser_Parents::ALaser_Parents()
	: bActive(false)
	, LaserColor(FLinearColor::Black)
	, bLaserHit(false)
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRootComp = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRootComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComp->SetupAttachment(SceneRootComp);
}

void ALaser_Parents::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALaser_Parents::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaser_Parents::SetLaserColor(FLinearColor Color)
{
	LaserColor = Color;
}

void ALaser_Parents::SetLaserSpawnerRef(ALaserSpawner* Spawner)
{
	LaserSpawnerRef = Spawner;
}

