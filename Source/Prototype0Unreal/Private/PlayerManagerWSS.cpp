// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerManagerWSS.h"
#include "MyCharacter.h"

void UPlayerManagerWSS::RegisterPlayer(AMyCharacter* player)
{
	Players.Add(player);
	SortPlayers();
	GEngine->AddOnScreenDebugMessage(-1, 1, player->GetPlayerColor(), FString::Printf(TEXT("Player %d was registered with manager"), player->PlayerIndex));
	//Players[player->PlayerIndex] = player;
}

void UPlayerManagerWSS::Initialize(FSubsystemCollectionBase& Collection)
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerManager Initialized"));
}

void UPlayerManagerWSS::SortPlayers()
{
	if (Players.Num() <= 1) {
		return;
	}
	for (int i = 1; i < Players.Num(); i++) {
		if (Players[i]->PlayerIndex > Players[i - 1]->PlayerIndex) {
			AMyCharacter* temp = Players[i - 1];
			Players[i - 1] = Players[i];
			Players[i] = temp;
		}
	}
}






