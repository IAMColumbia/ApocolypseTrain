// Fill out your copyright notice in the Description page of Project Settings.


#include "Vendor.h"

// Sets default values
AVendor::AVendor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVendor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AVendor::OnInteract(AMyCharacter* player)
{

}

// Called every frame
void AVendor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

