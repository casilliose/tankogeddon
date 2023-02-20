#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKOGEDDON_API UPhysicsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPhysicsComponent();
	TArray<FVector> GenerateTrajectory(FVector StartPos, FVector Velocity, float MaxTime, float TimeStep, float MinZValue = 0);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gravity params")
	float Gravity = -9.8f;
};
