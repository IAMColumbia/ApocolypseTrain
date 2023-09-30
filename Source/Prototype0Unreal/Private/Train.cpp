// Fill out your copyright notice in the Description page of Project Settings.



#include "Train.h"
#include "GameManagerWSS.h"


// Sets default values
ATrain::ATrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATrain::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetSubsystem<UGameManagerWSS>()->PrintTrainLocation(GetActorLocation());
}

// Called every frame
void ATrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	currentLocation = GetActorLocation();
	currentLocation += FVector(0, 1, 0) * trainSpeed * DeltaTime;
	SetActorLocation(currentLocation);
	GetWorld()->GetSubsystem<UGameManagerWSS>()->PrintTrainLocation(GetActorLocation());
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("target Pos = %f"), targetYPos));
	if (currentLocation.Y >= targetYPos) {
		targetYPos = GetWorld()->GetSubsystem<UGameManagerWSS>()->GetNextTargetLocation();
	}
}

