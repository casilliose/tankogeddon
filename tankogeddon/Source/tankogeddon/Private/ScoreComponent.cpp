// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreComponent.h"

UScoreComponent::UScoreComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UScoreComponent::AddScore(float Value)
{
	if (OnDestroyProjectile.IsBound()) {
		OnDestroyProjectile.Broadcast(Value);
	}
}