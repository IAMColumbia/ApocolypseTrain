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

	//when train reaches its current target location, call a method to this, that calls a method on the chunkspawner to spawn a chunk and get the next tagret location
	//maybe chunk spawner can be included in cpp and just wrap this in a method
	class AChunkSpawner* chunkSpawner;
	class AEnemySpawner* enemySpawner;
	class ATrain* train;

	void PrintTrainLocation(FVector location);
	float GetNextTargetLocation();
	void TrainArrivedAtTarget();

	FVector GetRandomLocationInChunk();

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
};
