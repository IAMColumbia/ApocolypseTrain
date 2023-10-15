// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameManagerWSS.generated.h"

/**
 * 
 */


UCLASS()
class PROTOTYPE0UNREAL_API UGameManagerWSS : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	class AChunkSpawner* chunkSpawner;
	class AEnemySpawner* enemySpawner;
	class ATrain* train;

	void PrintTrainLocation(FVector location);
	float GetNextTargetLocation();
	void TrainArrivedAtTarget();

	FVector GetRandomLocationInChunk();

	bool IsOutOfBounds(FVector actorLocation);

	UFUNCTION(BlueprintCallable)
	bool IsOutOfBackBounds(FVector actorLocation);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;


	
};
