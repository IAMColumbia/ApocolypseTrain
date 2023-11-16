// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PlayerWeapon.generated.h"

class AMyCharcater;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOTYPE0UNREAL_API UPlayerWeapon : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerWeapon();

	class AMyCharacter* OwnerCharacter;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Ray();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Attack(FColor color);

	
};
