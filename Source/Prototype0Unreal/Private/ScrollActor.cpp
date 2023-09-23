// Fill out your copyright notice in the Description page of Project Settings.


#include "ScrollActor.h"

// Sets default values for this component's properties
UScrollActor::UScrollActor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UScrollActor::BeginPlay()
{
	Super::BeginPlay();
	//currentLocation = root->GetComponentLocation();
	// ...
	
}


// Called every frame
void UScrollActor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

void UScrollActor::Scroll(float DeltaTime) {
	currentLocation.Y += scrollSpeed * DeltaTime;
	root->SetRelativeLocation(currentLocation);
}

