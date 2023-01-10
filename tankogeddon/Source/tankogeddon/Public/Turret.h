// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageTaker.h"
#include "GameStruct.h"
#include "HealthComponent.h"
#include "Turret.generated.h"

class UStaticMeshComponent;
class ACannon;

UCLASS()
class TANKOGEDDON_API ATurret : public AActor, public IDamageTaker
{
	GENERATED_BODY()
	
public:	
	ATurret();

	UFUNCTION()
	void TakeDamage(FDamageData DamageData);

protected:
	virtual void BeginPlay() override;
	void Targeting();
	void Destroyed();
	void RotateToPlayer();
	bool IsPlayerInRange();
	bool CanFire();
	void Fire();

	void SetupCannon();
	void DamageTaked(float Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* HitCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TSubclassOf<ACannon> CannonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* CannonSetupPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UHealthComponent* HealthComponent;

	UPROPERTY()
	ACannon* Cannon;

	UPROPERTY()
	APawn* PlayerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting range")
	float TargetingRange = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting range")
	float TargetingSpeed = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting range")
	float TargetingRate = 0.005f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting range")
	float Accurency = 10.0f;

	const FString BodyMeshPath = "/Script/Engine.StaticMesh'/Game/CSC/Meshes/SM_CSC_Tower1.SM_CSC_Tower1'";
	const FString TurretMeshPath = "/Script/Engine.StaticMesh'/Game/CSC/Meshes/SM_CSC_Gun1.SM_CSC_Gun1'";
};
