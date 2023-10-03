// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Train.generated.h"


UCLASS()
class PROTOTYPE0UNREAL_API ATrain : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrain();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool CanMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float targetYPos;

	FVector currentLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float trainSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float trainHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float TotalMeters;

	void IncrementTotalMeters();

	UFUNCTION(BlueprintCallable)
	void ResetTrain();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
