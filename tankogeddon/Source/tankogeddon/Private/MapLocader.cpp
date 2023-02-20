#include "MapLocader.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AMapLocader::AMapLocader()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceneComp;
	GatesMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gates Mesh"));
	GatesMesh->SetupAttachment(sceneComp);
	ActivatedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Activated lights"));
	ActivatedLight->SetupAttachment(sceneComp);

	DeactivatedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Deactivated lights"));
	DeactivatedLight->SetupAttachment(sceneComp);

	TriggerCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger collider"));
	TriggerCollider->SetupAttachment(sceneComp);
	TriggerCollider->OnComponentBeginOverlap.AddDynamic(this, &AMapLocader::OnTriggerOverlapBegin);

	SetActiveLights();
}

void AMapLocader::BeginPlay()
{
	Super::BeginPlay();
	SetActiveLights();
}

void AMapLocader::SetIsActivated(bool NewIsActivated)
{
	IsActivated = NewIsActivated;
	SetActiveLights();
}

void AMapLocader::SetActiveLights()
{
	ActivatedLight->SetHiddenInGame(!IsActivated);
	DeactivatedLight->SetHiddenInGame(IsActivated);
}

void AMapLocader::OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsActivated)
		return;
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	UE_LOG(LogTemp, Warning, TEXT("PlayerPawn  = %s"), *PlayerPawn->GetName());
	UE_LOG(LogTemp, Warning, TEXT("OtherActor %s"), *OtherActor->GetName());
	if (OtherActor == PlayerPawn)
	{
		UGameplayStatics::OpenLevel(GetWorld(), LoadLevelName);
	}
}
