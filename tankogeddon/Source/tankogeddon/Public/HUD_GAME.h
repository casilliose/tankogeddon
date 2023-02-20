// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUD_GAME.generated.h"

UENUM()
enum class EWidgetID : uint8
{
	None = 0x1,
	MainMenu = 0x2,
	PauseMenu = 0x3,
	PlayerInterface = 0x4,
	PlayerHp = 0x5,
	GameOver = 0x6,
};

UCLASS(Abstract)
class TANKOGEDDON_API AHUD_GAME : public AHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TMap<EWidgetID, TSubclassOf<UUserWidget>> WidgetClases;

	UPROPERTY()
	UUserWidget* CurrentWidget;

	EWidgetID CurrentWidgetID;

public:
	void BeginPlay() override;
	UUserWidget* UseWidget(EWidgetID widgetID, bool RemovePrevious = true, int32 ZOrder = 0);
	UUserWidget* GetCurrentWidget();
	void RemoveCurrentWidgetFromViewport();
	UUserWidget* AddWidgetToViewportByClass(TSubclassOf<UUserWidget> WidgetClass, int32 ZOrder = 0);
};
