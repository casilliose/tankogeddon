// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATankPlayerController();
	virtual void SetPawn(APawn* InPawn) override;
	
	FVector GetMousePos() {
		return MousePos;
	}

protected:
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	FVector MousePos;
	class ATankPawn* TankPawn;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void RotateRight(float Value);

	void Fire();

	void FireSpecial();
};
