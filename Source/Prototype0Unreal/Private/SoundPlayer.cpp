// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundPlayer.h"

// Sets default values for this component's properties
USoundPlayer::USoundPlayer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USoundPlayer::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (soundToPlay && actorTarget) {
		UGameplayStatics::PlaySoundAtLocation(this, soundToPlay, actorTarget->GetActorLocation());
	}
}


// Called every frame
void USoundPlayer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

