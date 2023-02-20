// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartMenu) {
		PlayAnimation(StartMenu);
	}
	
	if (NewGameBtn)
	{
		NewGameBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::OnNewGame);
	}

	if (LoadBtn)
	{
		LoadBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::OnLoadGame);
	}

	if (ExitBtn)
	{
		ExitBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitGame);
	}
}

void UMainMenuWidget::OnNewGame()
{
	UGameplayStatics::OpenLevel(this, "FirstLevel", true);
	FLatentActionInfo LatentInfo;
	UGameplayStatics::UnloadStreamLevel(this, "LVL_MainMenu", LatentInfo, false);
	this->RemoveFromViewport();
}

void UMainMenuWidget::OnLoadGame()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Еще в разработке")));
}

void UMainMenuWidget::OnExitGame()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand(TEXT("quit"));
}