// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameStruct.h"
#include "IScorable.h"
#include "MachineComponent.h"
#include "DamageTaker.h"

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* SceneComp = CreateDefaultSubobject <USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(SceneComp);

	USphereComponent* SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(ProjectileMesh);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);
	SphereCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

	ScoreComponent = CreateDefaultSubobject<UScoreComponent>(TEXT("ScoreComponent"));
}

void AProjectile::Start()
{
	GetWorld()->GetTimerManager().SetTimer(MoveTimer, this, &AProjectile::Move, MoveRate, true, MoveRate);
}

void AProjectile::Move()
{
	FVector movePosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(movePosition);
}

void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* owner = GetOwner();
	AActor* ownerByOwner = owner != nullptr ? owner->GetOwner() : nullptr;
	if (OtherActor != owner && OtherActor != ownerByOwner)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Damage overlap : %s"), *OtherActor->GetName());
		IIScorable* Scorable = Cast<IIScorable>(OtherActor);
		if (Scorable)
		{
			AMachineComponent* Machine = Cast<AMachineComponent>(OtherActor);
			if (Machine) {
				if (Machine->HealthComponent->GetHealth() <= 0) {
					ScoreComponent->AddScore(Scorable->GetScore());
				}
			}
		}
		IDamageTaker* DamageTakerActor = Cast<IDamageTaker>(OtherActor);
		if (DamageTakerActor) {
			FDamageData damageData;
			damageData.DamageValue = Damage;
			damageData.Instigator = GetOwner();
			damageData.DamageMaker = this;
			DamageTakerActor->TakeDamageMachine(damageData);
		}
		else
		{
			UPrimitiveComponent* mesh = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
			if (mesh)
			{
				if (mesh->IsSimulatingPhysics())
				{
					FVector forceVector = OtherActor->GetActorLocation() - GetActorLocation();
					forceVector.Normalize();
					mesh->AddImpulse(forceVector * PushForce, NAME_None, true);
				}
			}
		}
	}
	GetWorldTimerManager().ClearTimer(MoveTimer);
	Destroy();
}
