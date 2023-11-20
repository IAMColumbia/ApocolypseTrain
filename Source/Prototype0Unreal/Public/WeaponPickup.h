// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "WeaponPickup.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE0UNREAL_API AWeaponPickup : public AInteractableActor
{
	GENERATED_BODY()
	
public:
	virtual void OnInteract(AMyCharacter* player) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AWeapon> AvailableWeapon;

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyWeaponSwapped();

protected:
	virtual void BeginPlay() override;

	void AttachWeapon();

	USceneComponent* SpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool pickedUp;
};
