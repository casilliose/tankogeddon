// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageTaker.h"
#include "GameStruct.h"
#include "HealthComponent.h"
#include "MachineComponent.h"
#include "IScorable.h"
#include "Turret.generated.h"

class UStaticMeshComponent;
class ACannon;

UCLASS()
class TANKOGEDDON_API ATurret : public AMachineComponent, public IIScorable
{
	GENERATED_BODY()
	
public:
	ATurret();

protected:
	virtual void BeginPlay() override;
	void Targeting();
	void RotateToPlayer();
	bool IsPlayerInRange();
	bool CanFire();
	bool IsPlayerSeen();
	void DieEffect() override;

	UFUNCTION()
	float GetScore();

	UPROPERTY()
	APawn* PlayerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting params")
	float TargetingRange = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting params")
	float TargetingSpeed = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting params")
	float TargetingRate = 0.005f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting params")
	float Accurency = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting params")
	int ScoreValue = 2;


	const FString BodyMeshPath = "/Script/Engine.StaticMesh'/Game/CSC/Meshes/SM_CSC_Tower1.SM_CSC_Tower1'";
	const FString TurretMeshPath = "/Script/Engine.StaticMesh'/Game/CSC/Meshes/SM_CSC_Gun1.SM_CSC_Gun1'";

	FTimerHandle targetingTimerHandle;
};
