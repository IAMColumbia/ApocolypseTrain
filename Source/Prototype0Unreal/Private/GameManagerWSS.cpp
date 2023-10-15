// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManagerWSS.h"
#include "ChunkSpawner.h"
#include "Train.h"
#include "EnemySpawner.h"

void UGameManagerWSS::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("GameManager Initialized"));
}


void UGameManagerWSS::PrintTrainLocation(FVector location) {
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Magenta, FString::Printf(TEXT("Train Location %f"), location.Y));
}

float UGameManagerWSS::GetNextTargetLocation() {
	return chunkSpawner->GetNextTargetLocation();
}

void UGameManagerWSS::TrainArrivedAtTarget()
{
	chunkSpawner->SpawnNextChunk();
	if (train->CanMove) {
		enemySpawner->SpawnEnemies();
	}
	train->targetYPos = GetNextTargetLocation();
	train->IncrementTotalMeters();
}

FVector UGameManagerWSS::GetRandomLocationInChunk()
{
	//maybe just add the targetYpos to the y value on the vector to keep things in range. Spawnign on the x and z should be fine if just getting values from the mesh
	FVector chunkScale = chunkSpawner->chunkScale;
	float yOffset = chunkSpawner->nextTargetLocation;
	float zOffset = 400;
	FVector location = FVector(FMath::RandRange(-chunkScale.X, chunkScale.X), FMath::RandRange(-chunkScale.Y + yOffset, chunkScale.Y + yOffset), zOffset);
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, FString::Printf(TEXT("x: %f y: %f z: %f"), location.X, location.Y, location.Z));

	return location;
}

bool UGameManagerWSS::IsOutOfBounds(FVector actorLocation)
{
	if (actorLocation.Y < train->GetBackBound()|| actorLocation.Y > train->GetFrontBound()) {
		return true;
	}
	if (actorLocation.Z < -100) {
		return true;
	}
	return false;
}

bool UGameManagerWSS::IsOutOfBackBounds(FVector actorLocation)
{
	if (actorLocation.Y < train->GetBackBound()) {
		return true;
	}
	if (actorLocation.Z < -100) {
		return true;
	}
	return false;
}


