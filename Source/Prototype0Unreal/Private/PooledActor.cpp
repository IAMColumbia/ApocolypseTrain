// Fill out your copyright notice in the Description page of Project Settings.


#include "PooledActor.h"

// Sets default values
APooledActor::APooledActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APooledActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APooledActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APooledActor::Spawn()
{
	SetActorHiddenInGame(false);
}

void APooledActor::Despawn()
{
	SetActorHiddenInGame(true);
}

