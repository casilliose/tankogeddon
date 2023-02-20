// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Animation/WidgetAnimation.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;


protected:
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* NewGameBtn;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* LoadBtn;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* ExitBtn;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* StartMenu;
	
	UFUNCTION()
	void OnNewGame();

	UFUNCTION()
	void OnLoadGame();

	UFUNCTION()
	void OnExitGame();
};
