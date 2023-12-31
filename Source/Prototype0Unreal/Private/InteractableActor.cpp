// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"
#include <Components/SphereComponent.h>
#include <MyCharacter.h>
#include <PlayerManagerWSS.h>

// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AInteractableActor::DropObject(FVector directionToLaunch)
{
	state = EInteractableState::Dropped;
	OnDropped(directionToLaunch);
}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	TSet<UActorComponent*> components = GetComponents();
	for (UActorComponent* component : components) {
		if (component->IsA(USphereComponent::StaticClass())) {
			USphereComponent* SphereCollisionComponent = Cast<USphereComponent>(component);
			if (SphereCollisionComponent && SphereCollisionComponent->ComponentHasTag("Trigger"))
			{
				trigger = SphereCollisionComponent;
				trigger->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActor::OnTriggerBeginOverlap);
				trigger->OnComponentEndOverlap.AddDynamic(this, &AInteractableActor::OnTriggerEndOverlap);
				
			}
		}
	}
}

void AInteractableActor::CheckForInteractPressed()
{
	if (wasInteracted) {
		return;
	}
	for(AMyCharacter * player : overlappingPlayers) {
		if (player->Interacted && !player->justDropped) {
			OnInteract(player);
			
		}
	}
}

void AInteractableActor::OnInteract(AMyCharacter* player)
{
	if (!player->Carrying) {
		player->PickupItem(this);
		state = EInteractableState::Carried;
		return;
	}
}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for(AMyCharacter* c : GetWorld()->GetSubsystem<UPlayerManagerWSS>()->Players) {
		if (IsOverlappingActor(c) && !overlappingPlayers.Contains(c)) {
			overlappingPlayers.Add(c);
			PlayersOverlapping();
		}
	}
	if (state == EInteractableState::Idle) {
		if (overlappingPlayers.Num() > 0) {
			CheckForInteractPressed();
		}
	}
	for (AMyCharacter* c : GetWorld()->GetSubsystem<UPlayerManagerWSS>()->Players) {
		if (!IsOverlappingActor(c) && overlappingPlayers.Contains(c)) {
			overlappingPlayers.Remove(c);
			if (overlappingPlayers.Num() <= 0) {
				NoPlayersOverlapping();
			}
		}
	}
}

void AInteractableActor::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player")) {
		if (AMyCharacter* player = Cast<AMyCharacter>(OtherActor)) {
			/*overlappingPlayers.Add(player);
			PlayersOverlapping();*/
		}
	}
}

void AInteractableActor::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Player")) {
		if (AMyCharacter* player = Cast<AMyCharacter>(OtherActor)) {
			/*overlappingPlayers.Remove(player);
			if (overlappingPlayers.Num() <= 0) {
				NoPlayersOverlapping();
			}*/
		}

	}
}

