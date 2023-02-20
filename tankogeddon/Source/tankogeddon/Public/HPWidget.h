// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HPWidget.generated.h"

UCLASS()
class TANKOGEDDON_API UHPWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:	
	UPROPERTY(meta = (BindWidgetOptional))
	UProgressBar* progressBarHP;

public:
	void setCurrentHP(float score);
};
