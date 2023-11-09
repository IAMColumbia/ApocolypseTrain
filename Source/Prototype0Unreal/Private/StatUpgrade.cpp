// Fill out your copyright notice in the Description page of Project Settings.


#include "StatUpgrade.h"
#include "MyCharacter.h"



// Sets default values
AStatUpgrade::AStatUpgrade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStatUpgrade::BeginPlay()
{
	Super::BeginPlay();
	
}

FText getUpgradeText(float value, EUpgradeType type)
{
	return FText::Format(INVTEXT("+{0} {1}"), value, StaticEnum<EUpgradeType>()->GetDisplayNameTextByValue((uint64)type));
}


// Called every frame
void AStatUpgrade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!wasInteracted) {

	}
	if (overlappingPlayers.Num() <= 0 && OpenPercent > 0) {
		OpenPercent -= OpenRate;
	}

}

void AStatUpgrade::CheckForInteractPressed() {
	if (wasInteracted) {
		return;
	}
	if (activePlayer != NULL) {
		if (activePlayer->Interacted) {
			if (OpenPercent < OpenCompletePercent) {
				OpenPercent += OpenRate;
			}
			if (OpenPercent >= OpenCompletePercent) {
				OnInteract(activePlayer);
				wasInteracted = true;
			}
		}
		if (!activePlayer->Interacted) {
			activePlayer = NULL;
			ProgressColor = DefaultColor;
		}
		if (!overlappingPlayers.Contains(activePlayer)) {
			activePlayer = NULL;
		}
	}
	else {
		for (AMyCharacter* player : overlappingPlayers) {
			if (player->Interacted) {
				activePlayer = player;
				ProgressColor = activePlayer->PlayerColor;
			}
			
		}
	}
	
}

void AStatUpgrade::OnInteract(AMyCharacter* player)
{
	if (wasInteracted) {
		return;
	}
	wasInteracted = true;
	int random = (int)FMath::FRandRange(0, (float)StaticEnum<EUpgradeType>()->GetMaxEnumValue());
	EUpgradeType upgrade = (EUpgradeType)random;
	float amount = 10;
	switch (random) {
	case 0:
		player->ApplyUpgrade(upgrade, amount);
		Interacted(getUpgradeText(amount, upgrade), player->PlayerColor);
		break;
	case 1:
		player->ApplyUpgrade(upgrade, amount);
		Interacted(getUpgradeText(amount, upgrade), player->PlayerColor);
		break;
	case 2:
		player->ApplyUpgrade(upgrade, amount);
		Interacted(getUpgradeText(amount, upgrade), player->PlayerColor);
		break;
	}
	
}



