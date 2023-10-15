// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

//Maybe i can make this into a base class for both the chunk spawn and enemy spawner? Problem is enemies are ACharacter and chunks are AActor so I cant make them inherit a pooled object. I bet an interface could fix this
UCLASS()
class PROTOTYPE0UNREAL_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TQueue<AActor*> objectPool;

	virtual void PopulatePool();



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
