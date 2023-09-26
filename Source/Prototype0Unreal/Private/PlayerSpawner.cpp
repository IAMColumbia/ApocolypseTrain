// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSpawner.h"


// Sets default values
APlayerSpawner::APlayerSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}



// Called when the game starts or when spawned
void APlayerSpawner::BeginPlay()
{

	Super::BeginPlay();
	UGameplayStatics::CreatePlayer(this, -1, true);
	UGameplayStatics::CreatePlayer(this, -1, true);
	UGameplayStatics::CreatePlayer(this, -1, true);
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, TEXT("whatevertrejbkrj"));
}

// Called every frame
void APlayerSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

