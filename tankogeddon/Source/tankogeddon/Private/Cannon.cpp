// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"

ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = sceneComp;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	CannonMesh->SetupAttachment(sceneComp);
	
	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();
}

void ACannon::Fire()
{
	if (CountProjectile <= 0) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("clip is empty")));
		return;
	}
	if (!IsReadyToFire())
	{
		return;
	}
	bReadyToFire = false;
	if (CannonType == ECannonType::FireProjectile) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("FireProjectile")));
	}
	else 
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("FireTrace")));
	}
	CountProjectile--;
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, 1 / FireRate, false);
}

void ACannon::FireSpecial()
{
	//UE_LOG(LogTemp, Warning, TEXT("CountProjectile %f"), CountProjectile);
	if (CountProjectile <= 0) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("clip is empty")));
		return;
	}
	if (!IsReadyToFire())
	{
		return;
	}
	bReadyToFire = false;
	GetWorld()->GetTimerManager().SetTimer(QTimer, this, &ACannon::FireQ, IntervalBetweenFireInQ, true);
}

void ACannon::FireQ()
{
	if (countQFire < CountProjectileInQ) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("FireQ")));
		countQFire++;
		return;
	}
	GetWorld()->GetTimerManager().SetTimer(QTimer, this, &ACannon::FireQ, -1);
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, 1 / FireRate, false);
}

bool ACannon::IsReadyToFire()
{
	return bReadyToFire;
}

void ACannon::Reload()
{
	bReadyToFire = true;
	countQFire = 0;
}