// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraAdjust.h"
#include "PlayerManagerWSS.h"
#include "GameManagerWSS.h"

// Sets default values for this component's properties
UCameraAdjust::UCameraAdjust()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCameraAdjust::BeginPlay()
{
	Super::BeginPlay();
	playerManager = GetWorld()->GetSubsystem<UPlayerManagerWSS>();
	// ...
	smoothOffset = 2 * cameraSpeed;
}


// Called every frame
void UCameraAdjust::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	curLoc = GetOwner()->GetActorLocation();
	FVector loc = curLoc + FVector(0,GetYDir(),0) * cameraSpeed;
	GetOwner()->SetActorLocation(loc);
	// ...
}

float UCameraAdjust::GetYDir()
{
	float trainpos = GetWorld()->GetSubsystem<UGameManagerWSS>()->GetTrainLocation().Y;
	float targetloc = playerManager->GetAverageLocation();
	//keep camera from going to far
	if (targetloc < trainpos - TrainPositionOffset) {
		targetloc = trainpos - TrainPositionOffset;
	}
	else if (targetloc > trainpos + TrainPositionOffset) {
		targetloc = trainpos + TrainPositionOffset;
	}
	//smooth camera movements
	else if (targetloc < trainpos + deadZone || targetloc > trainpos - deadZone) {
		targetloc = trainpos;
	}
	//prevetn jittering over exact target location
	if (curLoc.Y < targetloc - smoothOffset) {
		return 1;
	}
	if (curLoc.Y > targetloc + smoothOffset) {
		return -1;
	}
	return 0;
}

