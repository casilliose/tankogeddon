// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DamageTaker.h"
#include "HealthComponent.h"
#include "GameStruct.h"
#include "MachineComponent.h"
#include "ScoreComponent.h"
#include "GameFramework/ForceFeedbackEffect.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/TargetPoint.h"
#include "Components/Widget.h"
#include "Components/WidgetComponent.h"
#include "TankPawn.generated.h"


class UStaticMeshComponent;
class ACannon;

UCLASS()
class TANKOGEDDON_API ATankPawn : public AMachineComponent 
{
	GENERATED_BODY()

public:
	ATankPawn();
	virtual void Tick(float DeltaTime) override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void RotateRight(float Value);

	void FireSpecial();
	void ChangeCannon();
	void AddCountProjectile(float CountProjectile);
	void DieEffect() override;
	bool isDie();
	void setDie();

	UFUNCTION()
	void AddScore(float Value);

	TArray<FVector> GetPatrollingPoints();
	void SetPatrollingPoints(TArray<ATargetPoint*> NewPatrollingPoints);
	
	UFUNCTION()
	float GetMovementAccurency() { return MovementAccurency; };

	UFUNCTION()
	FVector GetTurretForwardVector();

	UFUNCTION()
	void RotateTurretTo(FVector TargetPosition);
	
	UFUNCTION()
	FVector GetEyesPosition();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UScoreComponent* ScoreComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
	ACannon* CannonSecond;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
	TSubclassOf<ACannon> CannonSecondClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	float Score = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Patrolpoints" , Meta = (MakeEditWidget = true))
	TArray<ATargetPoint*> PatrollingPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Moveparams | Accurency")
	float MovementAccurency = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Params")
	bool isDieTank = false;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthWidget", Meta = (MakeEditWidget = true))
	//TSubclassOf<UUserWidget> HealthWidget;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthWidget", Meta = (MakeEditWidget = true))
	//UWidgetComponent* WidgetComp;

private:
	class ATankPlayerController* TankController;
	float ForwardMoveAxisValue = 0.0f;
	float RightMoveAxisValue = 0.0f;
	float CurrentRightAxisValue = 0.0f;
	float RotateRightAxisValue = 0.0f;
	float RotateInterpolationKey = 0.1f;
	float TurretInterpolationKey = 0.5f;
	
};
