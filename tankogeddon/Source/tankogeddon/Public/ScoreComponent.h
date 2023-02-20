// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScoreComponent.generated.h"

DECLARE_EVENT_OneParam(UScoreComponent, FOnDestroyProjectile, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKOGEDDON_API UScoreComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UScoreComponent();
	void AddScore(float Value);

	FOnDestroyProjectile OnDestroyProjectile;
};
