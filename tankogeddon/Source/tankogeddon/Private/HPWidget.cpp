// Fill out your copyright notice in the Description page of Project Settings.


#include "HPWidget.h"

void UHPWidget::setCurrentHP(float score)
{
	progressBarHP->SetPercent(score / 100);
}