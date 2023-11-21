// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Sniper.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE0UNREAL_API ASniper : public AWeapon
{
	GENERATED_BODY()

public:

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Penetration;

	virtual void Ray() override;
	
};
