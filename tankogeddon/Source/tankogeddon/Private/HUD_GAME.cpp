// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD_GAME.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


void AHUD_GAME::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld())
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PC, nullptr, EMouseLockMode::DoNotLock, false);
			PC->bShowMouseCursor = true;
		}
	}
}

UUserWidget* AHUD_GAME::UseWidget(EWidgetID widgetID, bool RemovePrevious, int32 ZOrder)
{
	if (CurrentWidgetID == widgetID) return CurrentWidget;
	if (RemovePrevious)
	{
		RemoveCurrentWidgetFromViewport();
	}
	TSubclassOf<UUserWidget> WidgetClassToUse = WidgetClases.FindRef(widgetID);
	if (WidgetClassToUse.Get())
	{
		CurrentWidgetID = widgetID;
		return AddWidgetToViewportByClass(WidgetClassToUse, ZOrder);
	}
	return nullptr;
}

UUserWidget* AHUD_GAME::GetCurrentWidget()
{
	return CurrentWidget;
}

void AHUD_GAME::RemoveCurrentWidgetFromViewport()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
		CurrentWidgetID = EWidgetID::None;
	}
}

UUserWidget* AHUD_GAME::AddWidgetToViewportByClass(TSubclassOf<UUserWidget> WidgetClass, int32 ZOrder)
{
	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	if (CurrentWidget)
	{
		CurrentWidget->AddToViewport(ZOrder);
	}
	return CurrentWidget;
}
