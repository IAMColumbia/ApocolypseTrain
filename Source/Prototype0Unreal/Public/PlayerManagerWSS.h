// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PlayerManagerWSS.generated.h"

/**
 * 
 */
class AMyCharacter;

UCLASS()
class PROTOTYPE0UNREAL_API UPlayerManagerWSS : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	TArray<AMyCharacter*> players;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
};
