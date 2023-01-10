// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Projectile.h"

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
	//UE_LOG(LogTemp, Warning, TEXT("Count patricy : %f"), CountProjectile);
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
		AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		if (projectile)
		{
			UE_LOG(LogTemp, Warning, TEXT("Turret faer 2"));
			projectile->Start();
		}
	}
	else 
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("FireTrace")));
		FHitResult hitResult;
		FCollisionQueryParams traceParams;
		traceParams.bTraceComplex = true;
		traceParams.bReturnPhysicalMaterial = false;

		FVector Start = ProjectileSpawnPoint->GetComponentLocation();
		FVector End = Start + ProjectileSpawnPoint->GetForwardVector() * FireRange;
		if (GetWorld()->LineTraceSingleByChannel(hitResult, Start, End, ECollisionChannel::ECC_Visibility, traceParams))
		{
			DrawDebugLine(GetWorld(), Start, hitResult.Location, FColor::Purple, false, 1.0f, 0, 5.0f);
			if (hitResult.GetActor())
			{
				UE_LOG(LogTemp, Warning, TEXT("currentLocation: %s"), *hitResult.GetActor()->GetName());
				hitResult.GetActor()->Destroy();
			}
		}
		else {
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 5.0f);
		}
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

void ACannon::AddCountProjectile(float AddCountProjectile)
{
	CountProjectile += AddCountProjectile;
}