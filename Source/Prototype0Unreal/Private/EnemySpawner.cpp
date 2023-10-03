// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "GameManagerWSS.h"
// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetSubsystem<UGameManagerWSS>()->enemySpawner = this;
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::SpawnEnemies()
{
	for (int i = 0; i < EnemiesPerChunk; i++) {
		FVector spawnLocation = GetWorld()->GetSubsystem<UGameManagerWSS>()->GetRandomLocationInChunk();
		NotifySpawnEnemy(spawnLocation);
	}
	EnemiesPerChunk++;
}

void AEnemySpawner::PrintStuff()
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, TEXT("I am enemy spawner"));
}

