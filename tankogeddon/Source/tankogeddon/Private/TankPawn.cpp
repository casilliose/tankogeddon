// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/ForceFeedbackEffect.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/Widget.h"
#include "HPWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/AudioComponent.h"


ATankPawn::ATankPawn()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BoxCollision);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	HealthComponent->OnDie.AddUObject(this, &ATankPawn::Destroyed);
	HealthComponent->OnHealthChanged.AddUObject(this, &ATankPawn::DamageTaked);

	ScoreComponent = CreateDefaultSubobject<UScoreComponent>(TEXT("ScoreComponent"));
	ScoreComponent->OnDestroyProjectile.AddUObject(this, &ATankPawn::AddScore);

	ShootEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Shoot effect"));
	ShootEffect->SetupAttachment(BodyMesh);

	AudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio effect"));
	AudioEffect->SetupAttachment(BodyMesh);

	//WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	//WidgetComp->SetWidgetClass(HealthWidget);
	//UHPWidget* HpProgress = Cast<UHPWidget>(WidgetComp->GetUserWidgetObject());
	//HpProgress->setCurrentHP(100.f);
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	TankController = Cast<ATankPlayerController>(GetController());
	SetupCannon(CannonClass);
	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;
	CannonSecond = GetWorld()->SpawnActor<ACannon>(CannonSecondClass, params);
	CannonSecond->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector currentLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector movePosition = currentLocation + ForwardVector * MovementSpeed * ForwardMoveAxisValue * DeltaTime;
	FVector RightVector = GetActorRightVector();

	movePosition = movePosition + RightVector * MovementSpeed * RightMoveAxisValue * DeltaTime;
	SetActorLocation(movePosition);
	
	CurrentRightAxisValue = FMath::Lerp(CurrentRightAxisValue, RotateRightAxisValue, RotateInterpolationKey);
	float yawRotation = CurrentRightAxisValue * RotationSpeed * DeltaTime;
	FRotator currentRotation = GetActorRotation();
	yawRotation += currentRotation.Yaw;
	FRotator newRotator = FRotator(0.0f, yawRotation, 0.0f);
	SetActorRotation(newRotator);

	if (TankController) {
		FVector MousePos = TankController->GetMousePos();
		RotateTurretTo(MousePos);
	}
}

void ATankPawn::MoveForward(float Value)
{
	ForwardMoveAxisValue = Value;

}

void ATankPawn::MoveRight(float Value)
{
	RightMoveAxisValue = Value;

}

void ATankPawn::RotateRight(float Value)
{
	RotateRightAxisValue = Value;
}

void ATankPawn::FireSpecial()
{
	if (Cannon)
	{
		Cannon->FireSpecial();
	}
}

void ATankPawn::ChangeCannon()
{
	auto TempCannon = Cannon;
	Cannon = CannonSecond;
	CannonSecond = TempCannon;
}

void ATankPawn::AddCountProjectile(float CountProjectile)
{
	if (Cannon)
	{
		Cannon->AddCountProjectile(CountProjectile);
	}
}

void ATankPawn::AddScore(float Value)
{
	Score += Value;
}

TArray<FVector> ATankPawn::GetPatrollingPoints()
{
	TArray<FVector> points;
	for (ATargetPoint* point : PatrollingPoints)
	{
		points.Add(point->GetActorLocation());
	}
	return points;
}

void ATankPawn::SetPatrollingPoints(TArray<ATargetPoint*> NewPatrollingPoints)
{
	PatrollingPoints = NewPatrollingPoints;
}

FVector ATankPawn::GetTurretForwardVector()
{
	return TurretMesh->GetForwardVector();
}

void ATankPawn::RotateTurretTo(FVector TargetPosition)
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPosition);
	FRotator currRotation = TurretMesh->GetComponentRotation();
	targetRotation.Pitch = currRotation.Pitch;
	targetRotation.Roll = currRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(currRotation, targetRotation, TurretInterpolationKey));
}

FVector ATankPawn::GetEyesPosition()
{
	return CannonSetupPoint->GetComponentLocation();
}

void ATankPawn::DieEffect()
{
	if (!TankController) {
		PrimaryActorTick.bCanEverTick = false;
		setDie();
		if (this->GetController()) {
			this->GetController()->StopMovement();
		}
		SetActorTickEnabled(false);
	}
	else 
	{
		this->setDie();
	}
}

bool ATankPawn::isDie()
{
	return isDieTank;
}

void ATankPawn::setDie()
{
	isDieTank = true;
}