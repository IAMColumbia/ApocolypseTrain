// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PrintActor.generated.h"

UCLASS()
class PROTOTYPE0UNREAL_API APrintActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APrintActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void PrintHello();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
