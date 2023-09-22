// Copyright Epic Games, Inc. All Rights Reserved.


#include "Prototype0UnrealGameModeBase.h"

void APrototype0UnrealGameModeBase::StartPlay() {
	Super::StartPlay();
	UGameplayStatics::CreatePlayer(this, -1, true);
	//UGameplayStatics::CreatePlayer(this, -1);
}