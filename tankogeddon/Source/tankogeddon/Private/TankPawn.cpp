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

ATankPawn::ATankPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(BoxCollision);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BoxCollision);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CannonSetupPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDie.AddUObject(this, &ATankPawn::Destroyed);
	HealthComponent->OnHealthChanged.AddUObject(this, &ATankPawn::DamageTaked);
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
	//UE_LOG(LogTemp, Warning, TEXT("currentLocation: %s"), *currentLocation.ToString());
	FVector ForwardVector = GetActorForwardVector();
	//UE_LOG(LogTemp, Warning, TEXT("ForwardVector: %s"), *ForwardVector.ToString());
	FVector movePosition = currentLocation + ForwardVector * MovementSpeed * ForwardMoveAxisValue * DeltaTime;
	FVector RightVector = GetActorRightVector();
	//UE_LOG(LogTemp, Warning, TEXT("RightVector: %s"), *RightVector.ToString());
	movePosition = movePosition + RightVector * MovementSpeed * RightMoveAxisValue * DeltaTime;
	//UE_LOG(LogTemp, Warning, TEXT("movePosition: %s"), *movePosition.ToString());
	SetActorLocation(movePosition);
	
	CurrentRightAxisValue = FMath::Lerp(CurrentRightAxisValue, RotateRightAxisValue, RotateInterpolationKey);
	float yawRotation = CurrentRightAxisValue * RotationSpeed * DeltaTime;
	FRotator currentRotation = GetActorRotation();
	yawRotation += currentRotation.Yaw;
	FRotator newRotator = FRotator(0.0f, yawRotation, 0.0f);
	SetActorRotation(newRotator);

	if (TankController) {
		FVector MousePos = TankController->GetMousePos();
		FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MousePos);
		FRotator turretRotation = TurretMesh->GetComponentRotation();
		targetRotation.Pitch = turretRotation.Pitch;
		targetRotation.Roll = turretRotation.Roll;
		TurretMesh->SetWorldRotation(FMath::Lerp(targetRotation, turretRotation, TurretInterpolationKey));
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

void ATankPawn::SetupCannon(TSubclassOf<ACannon> newCannon)
{
	if (!newCannon)
	{
		return;
	}
	if (Cannon)
	{
		Cannon->Destroy();
	}
	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(newCannon, params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void ATankPawn::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
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

void ATankPawn::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void ATankPawn::Die()
{
	if (Cannon)
	{
		Cannon->Destroy();
	}
	Destroy();
}

void ATankPawn::DamageTaked(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Tank %s taked damage %f, heals %f"), *GetName(), Value, HealthComponent->GetHealth());
}