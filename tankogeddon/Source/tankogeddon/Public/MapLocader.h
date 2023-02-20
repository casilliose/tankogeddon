// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/Actor.h"
#include "MapLocader.generated.h"

UCLASS()
class TANKOGEDDON_API AMapLocader : public AActor
{
	GENERATED_BODY()
	
public:	
	AMapLocader();
	void SetIsActivated(bool NewIsActivated);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* GatesMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* TriggerCollider;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UPointLightComponent* ActivatedLight;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UPointLightComponent* DeactivatedLight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level loading params")
	FName LoadLevelName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level loading params")
	bool IsActivated = false;

	void SetActiveLights();

	UFUNCTION()
	void OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
