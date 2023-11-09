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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentYPos;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int TotalMeters;

	void PrintTrainLocation(FVector location);
	float GetNextTargetLocation();
	void SpawnNewChunk();

	UFUNCTION(BlueprintCallable)
	FVector GetRandomLocationInChunk(float zoffset);

	FVector GetRandomLocationBehindTrain();

	FVector GetTrainLocation();

	bool IsOutOfBounds(FVector actorLocation);

	UFUNCTION(BlueprintCallable)
	bool IsOutOfBackBounds(FVector actorLocation);

	void OnTrainAccelerating();
	void OnTrainStopped();

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
};
