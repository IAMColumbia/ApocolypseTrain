// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	currentHealth = MaxObstacleHealth;
}

// Called every frame
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObstacle::DamageObstacle(float damage)
{
	currentHealth -= FGenericPlatformMath::Abs(damage);
	GEngine->AddOnScreenDebugMessage(1, 3, FColor::Red, FString::Printf(TEXT("TRAIN HIT OBSTACLE % f"), currentHealth));
	if (currentHealth <= 0) {
		AActor::Destroy();
	}
}

