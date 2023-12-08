// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ShotgunWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE0UNREAL_API AShotgunWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float shotArc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float raysPerShot;

protected:
	virtual void Ray() override;
	
	virtual void SpawnProjectile() override;

	void SpawnShotCosmetic(FRotator rot);
};
