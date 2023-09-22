// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/GameModeBase.h"
#include "Prototype0UnrealGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE0UNREAL_API APrototype0UnrealGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void StartPlay() override;
	
};
