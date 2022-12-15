// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStruct.h"
#include "Cannon.generated.h"

UCLASS()
class TANKOGEDDON_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:	
	ACannon();
	void Fire();
	bool IsReadyToFire();
	void Reload();
	void FireSpecial();
	void FireQ();

	FTimerHandle ReloadTimer;
	FTimerHandle QTimer;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* CannonMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	ECannonType CannonType = ECannonType::FireProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Params")
	float FireRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Params")
	float FireRange = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Params")
	float FireDamage = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Params")
	float CountProjectile = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Params")
	float CountProjectileInQ = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Params")
	float IntervalBetweenFireInQ = 0.3f;

private:
	bool bReadyToFire = false;
	float countQFire = 0;
};
