// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TankPawn.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Actor.h"
#include "MapLocader.h"
#include "TankFactory.generated.h"

class AMapLocader;

UCLASS()
class TANKOGEDDON_API ATankFactory : public AActor, public IDamageTaker
{
GENERATED_BODY()
public:
	ATankFactory();

	UFUNCTION()
	void TakeDamageMachine(FDamageData DamageData);

	void DamageTaked(float Value);

protected:
	virtual void BeginPlay() override;
	void SpawnNewTank();

	UFUNCTION()
	void Die();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BuildingMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* TankSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* HitCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	TSubclassOf<ATankPawn> SpawnTankClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn tanks params")
	float SpawnTankRate = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	TArray<ATargetPoint*> TankWayPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Loader")
	AMapLocader* LinkedMapLoader;

	const FString DestroyMeshPath = "/Script/Engine.StaticMesh'/Game/Core/Mesh/garage_destroy.garage_destroy'";

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent* ShootEffectDestroy;

	FTimerHandle _targetingTimerHandle;
};
