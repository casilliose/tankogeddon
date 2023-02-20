// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWidget.h"

void UPlayerWidget::setScore(float score)
{
	scoreText->SetText(FText::AsNumber(score));
}

void UPlayerWidget::setProjectile(float projectile)
{
	projectileText->SetText(FText::AsNumber(projectile));
}
