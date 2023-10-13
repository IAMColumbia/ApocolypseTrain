// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
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

	void BurnFuel();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool CanMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float targetYPos;

	FVector currentLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float trainSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float trainHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float TotalMeters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Fuel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxFuel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SpeedFuelMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float burnRate;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	FVector player0RespawnPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	FVector player1RespawnPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	FVector player2RespawnPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	FVector player3RespawnPos;

	void IncrementTotalMeters();


	bool IsOverlappingFuelBox(FVector actorPos);

	bool AddFuel();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UBoxComponent* fuelDeposit;

	FVector GetRandomRespawnPos();

	FVector GetRespawnPos(int PlayerIndex);

};
