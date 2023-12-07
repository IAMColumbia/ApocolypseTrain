// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkSpawner.h"
#include "GameManagerWSS.h"


// Sets default values
AChunkSpawner::AChunkSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChunkSpawner::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetSubsystem<UGameManagerWSS>()->chunkSpawner = this;
	chunkScale = FVector(100, 100, 100);
}


// Called every frame
void AChunkSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AChunkSpawner::GetNextTargetLocation()
{
	return nextTargetLocation;
}

void AChunkSpawner::SpawnNextChunk()
{
	spawnLevelChunk();
}

