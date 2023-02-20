// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "PlayerWidget.generated.h"

UCLASS()
class TANKOGEDDON_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* scoreText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* projectileText;
	
public:
	void setScore(float score);
	void setProjectile(float score);
};
