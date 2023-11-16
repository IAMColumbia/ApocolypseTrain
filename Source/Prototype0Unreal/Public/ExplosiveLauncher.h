// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ExplosiveLauncher.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE0UNREAL_API AExplosiveLauncher : public AWeapon
{
	GENERATED_BODY()

	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ExplosiveProjectile;

	virtual void BeginPlay() override;

protected:
	void ShootProjectile() override;
};
