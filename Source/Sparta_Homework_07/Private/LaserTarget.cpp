// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserTarget.h"

#include "Laser.h"

ALaserTarget::ALaserTarget()
	: SelectableType(SELECTABLE_TYPE::END)
{
}

void ALaserTarget::BeginPlay()
{
	Super::BeginPlay();

	Super::StaticMeshComp->SetColorParameterValueOnMaterials(TEXT("EmissiveColor"), BasicColor);
	Super::StaticMeshComp->SetScalarParameterValueOnMaterials(TEXT("BaseReflectFractionIn"), 0.05f);
	Super::StaticMeshComp->SetScalarParameterValueOnMaterials(TEXT("BaseReflectFractionIn_Op"), 0.04f);
	Super::StaticMeshComp->SetScalarParameterValueOnMaterials(TEXT("Exponentin"), 5.0f);
	Super::StaticMeshComp->SetScalarParameterValueOnMaterials(TEXT("Exponentin_Op"), -1.0f);
}

void ALaserTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALaserTarget::PuzzleSolved(FLinearColor HitLaserColor)
{
	if (bActive)
	{
		Super::StaticMeshComp->SetColorParameterValueOnMaterials(TEXT("EmissiveColor"), HitLaserColor);
		Super::StaticMeshComp->SetScalarParameterValueOnMaterials(TEXT("BaseReflectFractionIn"), -100.0f);
		Super::StaticMeshComp->SetScalarParameterValueOnMaterials(TEXT("BaseReflectFractionIn_Op"), 0.8f);
		Super::StaticMeshComp->SetScalarParameterValueOnMaterials(TEXT("Exponentin"), -3.0f);
		Super::StaticMeshComp->SetScalarParameterValueOnMaterials(TEXT("Exponentin_Op"), -1.0f);
	}
	else
	{
		Super::StaticMeshComp->SetColorParameterValueOnMaterials(TEXT("EmissiveColor"), BasicColor);
		Super::StaticMeshComp->SetScalarParameterValueOnMaterials(TEXT("BaseReflectFractionIn"), 0.05f);
		Super::StaticMeshComp->SetScalarParameterValueOnMaterials(TEXT("BaseReflectFractionIn_Op"), 0.04f);
		Super::StaticMeshComp->SetScalarParameterValueOnMaterials(TEXT("Exponentin"), 5.0f);
		Super::StaticMeshComp->SetScalarParameterValueOnMaterials(TEXT("Exponentin_Op"), -1.0f);
	}
}

ALaser* ALaserTarget::LaserBounce(ALaser* LaserHit, bool HitResponse, const FHitResult& HitInformation)
{
	bActive = HitResponse;
	
	PuzzleSolved(LaserHit->GetLaserColor());

	return nullptr;
}
