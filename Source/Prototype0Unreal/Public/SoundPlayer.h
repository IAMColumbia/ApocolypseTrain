// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SoundPlayer.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOTYPE0UNREAL_API USoundPlayer : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USoundPlayer();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	AActor* actorTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Audio")
	USoundBase* soundToPlay;
};
