// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChunkSpawner.generated.h"

class UGameManagerWSS;

UCLASS()
class PROTOTYPE0UNREAL_API AChunkSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunkSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void spawnLevelChunk();

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spawning")
	FVector chunkScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subsystem")
	float nextTargetLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spawning")
	int TotalChunksSpawned;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float GetNextTargetLocation();

	void SpawnNextChunk();

};
