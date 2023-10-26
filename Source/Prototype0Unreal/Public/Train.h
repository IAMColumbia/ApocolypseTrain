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

	enum TrainState { stopped, accelerating, decelerating };
	enum LeverType { stopLever, startLever};

	TrainState currentState;

	void StartTrain();

	void StopTrain();

	void MovementUpdate();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
	float MaxTrainSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float AccelerationRate;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float DecelerationRate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void BurnFuel();

	//the back bound of the train to cleanup enemies players and fuel that are left behind. this is relative to the trains world location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounds")
	float BackBound;
	//stop players from going to far forward. this is relative to the trains world location.
	UPROPERTY(EditAnywhere, Category = "Bounds")
	float FrontBound;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool CanMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float targetYPos;

	FVector currentLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float currentTrainSpeed;

	//damage dealt to enemy when train runs into it, multiplied by speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float damageMultiplier;

	//force enemy launched by train runs into it, multiplied by speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float launchMultiplier;

	//minnimum speed the train must be going to deal damage or launch enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float minnimumDamagingSpeed;

	UPROPERTY(VisibleAnywhere)
	int TotalMeters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Fuel;

	//Maximum fuel that can be in the train at once. this affects max speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxFuel;

	//how much the integer number fuel is multiplied by to set the speed of the train. this affects speed and max speed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SpeedFuelMultiplier;

	//the decceleration rate of the train. how fast does the train burn fuel.
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

	bool IsOverlappingLeverBox(FVector actorPos, LeverType type);

	bool AddFuel();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float GetBackBound();

	float GetFrontBound();

	UBoxComponent* fuelDeposit;
	UBoxComponent* startBox;
	UBoxComponent* stopBox;
	UBoxComponent* plow;

	UStaticMeshComponent* startLeverMesh;

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

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyStartLeverOverlap(bool isOverlapping);

};
