
#include "TankFactory.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "TankPawn.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"


ATankFactory::ATankFactory()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceneComp;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Mesh"));
	BuildingMesh->SetupAttachment(sceneComp);

	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	TankSpawnPoint->AttachToComponent(sceneComp,FAttachmentTransformRules::KeepRelativeTransform);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(sceneComp);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDie.AddUObject(this, &ATankFactory::Die);
	HealthComponent->OnHealthChanged.AddUObject(this, &ATankFactory::DamageTaked);

	ShootEffectDestroy = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ShootEffectDestroy"));
	ShootEffectDestroy->SetupAttachment(BuildingMesh);
}

void ATankFactory::BeginPlay()
{
	Super::BeginPlay();
	if (LinkedMapLoader)
		LinkedMapLoader->SetIsActivated(false);
	GetWorld()->GetTimerManager().SetTimer(_targetingTimerHandle, this, &ATankFactory::SpawnNewTank, SpawnTankRate, true, SpawnTankRate);
}

void ATankFactory::DamageTaked(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Fabrica %s taked damage %f, heals %f"), *GetName(), Value, HealthComponent->GetHealth());
}

void ATankFactory::Die()
{
	if (LinkedMapLoader)
		LinkedMapLoader->SetIsActivated(true);
	UStaticMesh* DestroyMesh = LoadObject<UStaticMesh>(this, *DestroyMeshPath);
	BuildingMesh->SetStaticMesh(DestroyMesh);
	if (ShootEffectDestroy)
	{
		ShootEffectDestroy->SetVisibility(true);
		ShootEffectDestroy->ActivateSystem();
	}
	GetWorldTimerManager().ClearTimer(_targetingTimerHandle);
	//Destroy();
}

void ATankFactory::TakeDamageMachine(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void ATankFactory::SpawnNewTank()
{
	FTransform spawnTransform(TankSpawnPoint->GetComponentRotation(),TankSpawnPoint->GetComponentLocation(), FVector(1));
	ATankPawn* newTank = GetWorld()->SpawnActorDeferred<ATankPawn>(SpawnTankClass, spawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	newTank->SetPatrollingPoints(TankWayPoints);
	newTank->SpawnDefaultController();
	UGameplayStatics::FinishSpawningActor(newTank, spawnTransform);
}
