// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "GameFramework/Actor.h"
#include "Vendor.generated.h"

UCLASS()
class PROTOTYPE0UNREAL_API AVendor : public AInteractableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVendor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnInteract(AMyCharacter* player) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
