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


	UPROPERTY(EditAnywhere, Category = "Bounds")
	float BackBound;
	UPROPERTY(EditAnywhere, Category = "Bounds")
	float FrontBound;

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
	int TotalMeters;

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

	float GetBackBound();

	float GetFrontBound();

	UBoxComponent* fuelDeposit;
	UBoxComponent* plow;

	FVector GetRandomRespawnPos();

	FVector GetRespawnPos(int PlayerIndex);

	UFUNCTION()
	void OnPlowBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void OnControllerPosses(int PlayerIndex, FColor playerColor);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTimer(int PlayerIndex, int timeLeft);

	UFUNCTION(BlueprintImplementableEvent)
	void StartRespawnTimer(int PlayerIndex, int timeLeft);

	UFUNCTION(BlueprintImplementableEvent)
	void StopRespawnTimer(int PlayerIndex, int timeLeft);

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyIncrementMeters(int meters);

};
