// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "GameManagerWSS.h"
#include "EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include <Kismet/KismetSystemLibrary.h>
// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TSet<UActorComponent*> components = GetComponents();
	for (UActorComponent* component : components) {
		if (component->IsA(UCapsuleComponent::StaticClass())) {
			UCapsuleComponent* capsule = Cast<UCapsuleComponent>(component);
			if (capsule && capsule->ComponentHasTag("Capsule"))
			{
				enemyHeight = capsule->GetScaledCapsuleHalfHeight();
			}
		}
	}
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

void AEnemySpawner::StartRearSpawner()
{
	GetWorld()->GetTimerManager().SetTimer(rearSpawner, this, &AEnemySpawner::SpawnEnemyBehindTrain, RearSpawnRate, true);
}

void AEnemySpawner::StopRearSpawner()
{
	GetWorld()->GetTimerManager().ClearTimer(rearSpawner);
}

void AEnemySpawner::SpawnEnemies()
{
	for (int i = 0; i < EnemiesPerChunk; i++) {
		FVector spawnLocation = GetWorld()->GetSubsystem<UGameManagerWSS>()->GetRandomLocationInChunk(enemyHeight);
		NotifySpawnEnemy(spawnLocation, FRotator().ZeroRotator, false, FVector());
	}
	EnemiesPerChunk++;
}

void AEnemySpawner::SpawnEnemyBehindTrain()
{
	FVector spawnLoc = GetWorld()->GetSubsystem<UGameManagerWSS>()->GetRandomLocationBehindTrain();
	NotifySpawnEnemy(spawnLoc, FRotator().ZeroRotator, true, GetWorld()->GetSubsystem<UGameManagerWSS>()->GetTrainLocation());
}

void AEnemySpawner::PrintStuff()
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, TEXT("I am enemy spawner"));
}

void AEnemySpawner::IncreaseEnemyDifficulty()
{
	EnemySpeed += EnemySpeedIncrease;
	EnemyHealth += EnemyHealthIncrease;
	EnemyDamage += EnemyDamageIncrease;
}

