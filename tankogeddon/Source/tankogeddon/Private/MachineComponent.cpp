// Fill out your copyright notice in the Description page of Project Settings.


#include "MachineComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Cannon.h"
#include "GameFramework/ForceFeedbackEffect.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"

AMachineComponent::AMachineComponent()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(BoxCollision);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CannonSetupPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	ShootEffectDestroy = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ShootEffectDestroy"));
	ShootEffectDestroy->SetupAttachment(BodyMesh);

	AudioEffectDestroy = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Effect Destroy"));
	AudioEffectDestroy->SetupAttachment(BodyMesh);
}

void AMachineComponent::BeginPlay()
{
	Super::BeginPlay();
}

void AMachineComponent::SetupCannon(TSubclassOf<ACannon> newCannon)
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

void AMachineComponent::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}

void AMachineComponent::DamageTaked(float Value)
{
	if (ShootEffect) {
		ShootEffect->ActivateSystem();
	}
	if (AudioEffect) {
		AudioEffect->Play();
	}
	UE_LOG(LogTemp, Warning, TEXT("%s taked damage %f, heals %f"), *GetName(), Value, HealthComponent->GetHealth());
}

void AMachineComponent::Destroyed()
{
	if (ShootEffectDestroy) {
		ShootEffectDestroy->SetVisibility(true);
		ShootEffectDestroy->ActivateSystem();
	}
	if (AudioEffectDestroy) {
		AudioEffectDestroy->Play();
	}
	if (Cannon)
	{
		Cannon->Destroy();
	}
	DieEffect();
	//Destroy();
}

void AMachineComponent::DieEffect()
{

}

void AMachineComponent::TakeDamageMachine(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}