// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerManagerWSS.h"
#include "MyCharacter.h"
#include <Kismet/KismetMathLibrary.h>
#include <Components/BoxComponent.h>
#include <Kismet/GameplayStatics.h>

void UPlayerManagerWSS::RegisterPlayer(AMyCharacter* player)
{
	Players.Add(player);
	SortPlayers();
	GEngine->AddOnScreenDebugMessage(-1, 1, player->GetPlayerColor(), FString::Printf(TEXT("Player %d was registered with manager"), player->PlayerIndex));
	//Players[player->PlayerIndex] = player;
}

bool UPlayerManagerWSS::IsOverlappingPlayer(UBoxComponent* box)
{
	for (int i = 0; i < Players.Num(); i++) {
		if (UKismetMathLibrary::IsPointInBox(Players[i]->GetActorLocation(), box->GetComponentLocation(), box->GetScaledBoxExtent()))
		{
			return true;
		}
	}
	return false;
}

bool UPlayerManagerWSS::IsOverlappingPlayerWithFuel(UBoxComponent* box)
{
	for (int i = 0; i < Players.Num(); i++) {
		if (UKismetMathLibrary::IsPointInBox(Players[i]->GetActorLocation(), box->GetComponentLocation(), box->GetScaledBoxExtent()))
		{
			if (Players[i]->HasFuel()) {
				return true;
			}
		}
	}
	return false;
}

int UPlayerManagerWSS::NumActivePlayers()
{
	return Players.Num();
}

void UPlayerManagerWSS::CheckGameOver()
{
	if (AllPlayersDead()) {
		float delayTime = 8;
		GEngine->AddOnScreenDebugMessage(-1, delayTime, FColor::Red, FString::Printf(TEXT("GAME OVER \nALL PLAYERS DIED")), true, FVector2D(10, 10));
		UGameplayStatics::OpenLevel((UObject*)GetWorld(), FName(TEXT("TrainTest")));
	}
}

bool UPlayerManagerWSS::AllPlayersDead()
{
	int deadPlayers = 0;
	int activePlayers = NumActivePlayers();
	if (activePlayers > 0) {
		for (int i = 0; i < activePlayers; i++) {
			if (Players[i]->IsPlayerDead == true) {
				deadPlayers++;
			}
		}
	}
	if (deadPlayers >= activePlayers) {
		return true;
	}
	return false;
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






