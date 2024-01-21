// Fill out your copyright notice in the Description page of Project Settings.


#include "LootBox.h"
#include "MyCharacter.h"
#include "GameManagerWSS.h"



// Sets default values
ALootBox::ALootBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALootBox::BeginPlay()
{
	Super::BeginPlay();
	
}

FText getUpgradeText(float value, EUpgradeType type)
{
	return FText::Format(INVTEXT("+{0} {1}"), value, StaticEnum<EUpgradeType>()->GetDisplayNameTextByValue((uint64)type));
}


// Called every frame
void ALootBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!wasInteracted) {

	}
	if (activePlayer== NULL && OpenPercent > 0) {
		OpenPercent -= OpenRate;
	}

}

void ALootBox::CheckForInteractPressed() {
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
			if (!startedInteract) {
				startedInteract = true;
				BeginInteract();
			}
			//GEngine->AddOnScreenDebugMessage(1, 3, activePlayer->GetPlayerColor(), TEXT("Interacted"));
		}
		if (!activePlayer->Interacted || !overlappingPlayers.Contains(activePlayer)) {
			activePlayer = NULL;
			startedInteract = false;
			ProgressColor = DefaultColor;
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

void ALootBox::OnInteract(AMyCharacter* player)
{
	if (wasInteracted) {
		return;
	}
	wasInteracted = true;
	int random = (int)FMath::FRandRange(0, (float)StaticEnum<EUpgradeType>()->GetMaxEnumValue());
	EUpgradeType upgrade = (EUpgradeType)random;
	float amount = UpgradeAmount;
	amount += GetWorld()->GetSubsystem<UGameManagerWSS>()->TotalChunksSpawned();
	switch (random) {
	case 0:
		player->ApplyUpgrade(upgrade, amount);
		Interacted(getUpgradeText(amount, upgrade), player->PlayerColor);
		break;
	case 1:
		player->ApplyUpgrade(upgrade, 20);
		Interacted(getUpgradeText(20, upgrade), player->PlayerColor);
		break;
	case 2:
		player->ApplyUpgrade(upgrade, 15);
		Interacted(getUpgradeText(15, upgrade), player->PlayerColor);
		break;
	}
	
}



