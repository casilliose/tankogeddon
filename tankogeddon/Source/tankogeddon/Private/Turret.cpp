// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/StaticMesh.h"
#include "Cannon.h"
#include "Kismet/KismetMathLibrary.h"

ATurret::ATurret()
{
	UStaticMesh* bodyMeshTemp = LoadObject<UStaticMesh>(this, *BodyMeshPath);
	if (bodyMeshTemp)
	{
		BodyMesh->SetStaticMesh(bodyMeshTemp);
	}

	UStaticMesh* turretMeshTemp = LoadObject<UStaticMesh>(this, *TurretMeshPath);
	if (turretMeshTemp)
	{
		TurretMesh->SetStaticMesh(turretMeshTemp);
	}

	HealthComponent->OnDie.AddUObject(this, &ATurret::Destroyed);
	HealthComponent->OnHealthChanged.AddUObject(this, &ATurret::DamageTaked);
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();
	SetupCannon(CannonClass);
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	GetWorld()->GetTimerManager().SetTimer(targetingTimerHandle, this, &ATurret::Targeting, TargetingRate, true, TargetingRate);
}

void ATurret::Targeting()
{
	if (!PlayerPawn)
	{
		return;
	}

	if (!IsPlayerSeen()) {
		return;
	}
		
	if (IsPlayerInRange())
	{
		RotateToPlayer();
		if (CanFire())
		{
			if (Cannon)
			{
				Fire();
			}
		}
	}
}

void ATurret::RotateToPlayer()
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
	FRotator currentRotation = TurretMesh->GetComponentRotation();
	targetRotation.Pitch = currentRotation.Pitch;
	targetRotation.Roll = currentRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(currentRotation, targetRotation, TargetingSpeed));
}

bool ATurret::IsPlayerInRange()
{
	return FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) <= TargetingRange;
}

bool ATurret::CanFire()
{
	FVector targetingDir = TurretMesh->GetForwardVector();
	FVector dirToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	dirToPlayer.Normalize();

	float AimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(targetingDir, dirToPlayer)));
	return AimAngle <= Accurency;
}

float ATurret::GetScore()
{
	return ScoreValue;
}

bool ATurret::IsPlayerSeen()
{
	FVector playerPos = PlayerPawn->GetActorLocation();
	FVector eyesPos = CannonSetupPoint->GetComponentLocation();
	FHitResult hitResult;
	FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	traceParams.bTraceComplex = true;
	traceParams.AddIgnoredActor(Cannon);
	traceParams.bReturnPhysicalMaterial = false;
	if (GetWorld()->LineTraceSingleByChannel(hitResult, eyesPos, playerPos, ECollisionChannel::ECC_Visibility, traceParams))
	{
		if (hitResult.GetActor())
		{
			return hitResult.GetActor() == PlayerPawn;
		}
	}
	return false;
}

void ATurret::DieEffect()
{
	PrimaryActorTick.bCanEverTick = false;
	GetWorldTimerManager().ClearTimer(targetingTimerHandle);
}