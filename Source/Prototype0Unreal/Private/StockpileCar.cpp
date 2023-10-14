// Fill out your copyright notice in the Description page of Project Settings.


#include "StockpileCar.h"

// Sets default values
AStockpileCar::AStockpileCar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStockpileCar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStockpileCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

