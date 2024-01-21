// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManagerWSS.h"
#include "ChunkSpawner.h"
#include "Train.h"
#include "EnemySpawner.h"
#include <Kismet/GameplayStatics.h>

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

void UGameManagerWSS::SpawnNewChunk()
{
	chunkSpawner->SpawnNextChunk();
	TotalMeters = train->TotalMeters;
	if (train->CanMove && chunkSpawner->TotalChunksSpawned > 0) {
		if (TotalMeters % enemySpawner->DifficultyIncrease == 0 && TotalMeters > 2) {
			enemySpawner->IncreaseEnemyDifficulty();
		}
		enemySpawner->SpawnEnemies();
	}
	train->targetYPos = GetNextTargetLocation();
	train->IncrementTotalMeters();
	CurrentYPos = train->targetYPos;
	
}


FVector UGameManagerWSS::GetRandomLocationInChunk(float zoffset)
{
	//maybe just add the targetYpos to the y value on the vector to keep things in range. Spawnign on the x and z should be fine if just getting values from the mesh
	if (chunkSpawner == NULL) {
		return FVector(0, 0, 0);
	}
	FVector chunkScale = chunkSpawner->chunkScale;
	float yOffset = chunkSpawner->nextTargetLocation;
	float zOffset = zoffset;
	FVector location = FVector(FMath::RandRange(-chunkScale.X, chunkScale.X), FMath::RandRange(-chunkScale.Y + yOffset, chunkScale.Y + yOffset), zOffset);
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, FString::Printf(TEXT("x: %f y: %f z: %f"), location.X, location.Y, location.Z));

	return location;
}

FVector UGameManagerWSS::GetRandomLocationBehindTrain()
{
	FVector loc = GetRandomLocationInChunk(100); 
	loc.Y = train->GetBackBound() + 500;
	return loc;
}

FVector UGameManagerWSS::GetTrainLocation()
{

	return train->GetActorLocation();
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
	if (train == NULL) {
		return false;
	}
	if (actorLocation.Y < train->GetBackBound()) {
		return true;
	}
	if (actorLocation.Z < -100) {
		return true;
	}
	return false;
}

void UGameManagerWSS::OnTrainAccelerating()
{
	enemySpawner->StopRearSpawner();
}

void UGameManagerWSS::OnTrainStopped()
{
	if (chunkSpawner->TotalChunksSpawned > 1) {
		enemySpawner->StartRearSpawner();
	}
}

bool UGameManagerWSS::TrainFuelFull()
{
	if (train != NULL) {
		if (train->Fuel >= train->MaxFuel) {
			return true;
		}
	}
	return false;
}

bool UGameManagerWSS::TrainFuelCritical()
{
	return train->FuelIsCritical();
}

int UGameManagerWSS::TotalChunksSpawned()
{
	if (chunkSpawner == NULL) {
		return 0;
	}
	return chunkSpawner->TotalChunksSpawned;
}

void UGameManagerWSS::GameOver(int loseType)
{
	if (!gameEnded) {
		gameEnded = true;
		float delayTime = 8;
		if (loseType == 0) {
			GEngine->AddOnScreenDebugMessage(-1, delayTime, FColor::Red, FString::Printf(TEXT("Game Over \nTrain Out Of Fuel")), true, FVector2D(10, 10));
		}
		if (loseType == 1) {
			GEngine->AddOnScreenDebugMessage(-1, delayTime, FColor::Red, FString::Printf(TEXT("Game Over \nAll Players Died")), true, FVector2D(10, 10));
		}
		
		FTimerHandle resetTimer;
		GetWorld()->GetTimerManager().SetTimer(resetTimer, this, &UGameManagerWSS::RestartGame, delayTime, true);
	}
}

void UGameManagerWSS::RestartGame()
{
	UGameplayStatics::OpenLevel((UObject*)GetWorld(), FName(TEXT("TrainTest")));
}

void UGameManagerWSS::PlayerJoined()
{
	if (train != NULL) {
		train->PlayerJoined();
	}
}


