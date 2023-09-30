// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChunkSpawner.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subsystem")
	float nextTargetLocation;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float GetNextTargetLocation();

};
