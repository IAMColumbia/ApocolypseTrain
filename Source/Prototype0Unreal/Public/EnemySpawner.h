// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class UGameManagerWSS;
class AEnemyCharacter;

UCLASS()
class PROTOTYPE0UNREAL_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
	float EnemyHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
	float EnemySpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
	float EnemyHealthIncrease;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
	float EnemySpeedIncrease;

	//a wave divisible by this number will increase the enemy difficulty
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
	int DifficultyIncrease;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void NotifySpawnEnemy(FVector spawnLocation, FRotator rotation, bool SetTarget, FVector target);

	//TQueue<AEnemyCharacter*> objectPool;
	//void PopulatePool();

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int EnemiesPerChunk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int MaxEnemies;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnEnemies();

	void SpawnEnemyBehindTrain();

	void PrintStuff();
	
	FTimerHandle spawnTimerHandle;

	void IncreaseEnemyDifficulty();

};
