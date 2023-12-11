// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickup.h"
#include "Weapon.h"
#include "MyCharacter.h"

void AWeaponPickup::OnInteract(AMyCharacter* player)
{
	if (!pickedUp) {
		AvailableWeapon = player->PickupWeapon(AvailableWeapon);
		pickedUp = true;
		pickingPlayer = player;
		NotifyWeaponSwapped();
	}
}

void AWeaponPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (pickingPlayer != NULL) {
		if (!pickingPlayer->Interacted) {
			pickedUp = false;
			pickingPlayer = NULL;
		}
	}
}

void AWeaponPickup::BeginPlay()
{
	Super::BeginPlay();
	TSet<UActorComponent*> components = GetComponents();
	for (UActorComponent* component : components) {
		if (component->IsA(USceneComponent::StaticClass())) {
			USceneComponent* sceneComp = Cast<USceneComponent>(component);
			if (sceneComp && sceneComp->ComponentHasTag("SpawnPoint"))
			{
				SpawnPoint = sceneComp;
			}
		}
	}
		//AvailableWeapon = Cast<AWeapon>(GetWorld()->SpawnActorAbsolute(DefaultWeapon, SpawnPoint->GetComponentTransform()));
}

void AWeaponPickup::AttachWeapon()
{
	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	//AvailableWeapon->SetActorLocation(SpawnPoint->GetComponentLocation());
	//AvailableWeapon->AttachToComponent(SpawnPoint, rules);
}
