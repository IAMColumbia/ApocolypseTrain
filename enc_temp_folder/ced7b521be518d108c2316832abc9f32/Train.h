// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include "Train.generated.h"


UENUM()
enum class ETrainState : uint8 {
	starting UMETA(DisplayName = "Starting"), stopped UMETA(DIsplayName = "Stopped"), accelerating UMETA(DIsplayName = "Accelerating"), decelerating UMETA(DIsplayName = "Decelerating")
};

UENUM()
enum class EFuelState : uint8 {
	normal UMETA(DIsplayName = "Normal"), critical UMETA(DIsplayName = "Critical")
};

UENUM()
enum class ELeverState : uint8 { move UMETA(DisplayName="Move"), stop UMETA(DisplayName="Stop") };

UCLASS()
class PROTOTYPE0UNREAL_API ATrain : public APawn
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	ATrain();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ELeverState leverState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EFuelState FuelState;

	UFUNCTION(BLueprintImplementableEvent)
	void NotifyFuelCritical();

	UFUNCTION(BLueprintImplementableEvent)
	void NotifyFuelNormal();
	

	void StartTrain();
	void StopTrain();

	void MovementUpdate();

	bool HasFuel();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
	float MaxTrainSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tutorial")
	float tutorialMaxTrainSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float AccelerationRate;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float DecelerationRate;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ETrainState currentState;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void BurnFuel();

	void UpdateFuelState();

	//the back bound of the train to cleanup enemies players and fuel that are left behind. this is relative to the trains world location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounds")
	float BackBound;
	//stop players from going to far forward. this is relative to the trains world location.
	UPROPERTY(EditAnywhere, Category = "Bounds")
	float FrontBound;

public:

	FTimerHandle countdown;
	void DecrementGameOverCounter();
	int graceTimeLeft;
	UFUNCTION(BlueprintImplementableEvent)
	void NotifyGameOverCounter();


	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool CanMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	float TutorialEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float targetYPos;

	FVector currentLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float currentTrainSpeed;

	//damage dealt to enemy when train runs into it, multiplied by speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float damageMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float playerDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float obstacleDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float playerLaunchForce;

	//force enemy launched by train runs into it, multiplied by speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float launchMultiplier;

	//minnimum speed the train must be going to deal damage or launch enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float minnimumDamagingSpeed;

	UPROPERTY(VisibleAnywhere)
	int TotalMeters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fuel")
	float Fuel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fuel")
	float CollisionFuelLoss;

	//Maximum fuel that can be in the train at once. this affects max speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fuel")
	float MaxFuel;

	//how much the integer number fuel is multiplied by to set the speed of the train. this affects speed and max speed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fuel")
	float SpeedFuelMultiplier;

	//the decceleration rate of the train. how fast does the train burn fuel.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fuel")
	float burnRate;

	bool isReversing;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	FVector player0RespawnPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	FVector player1RespawnPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	FVector player2RespawnPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	FVector player3RespawnPos;

	FTimerHandle startTimerHandle;
	void TrainCanMove();

	void IncrementTotalMeters();


	bool IsOverlappingFuelBox(FVector actorPos);

	bool IsOverlappingLeverBox(FVector actorPos);

	UFUNCTION(BLueprintImplementableEvent)
	void LeverStateChanged();

	void ToggleTrainState();

	UFUNCTION(BlueprintCallable)
	bool AddFuel();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float GetBackBound();

	float GetFrontBound();

	UBoxComponent* fuelDeposit;
	UBoxComponent* leverBox;
	UBoxComponent* plow;

	UStaticMeshComponent* startLeverMesh;

	FVector GetRandomRespawnPos();

	FVector GetRespawnPos(int PlayerIndex);

	//used to make sure certain behavior happens when the train state is updated
	UFUNCTION()
	void SetTrainState(ETrainState stateToSet);

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

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyTrainHitObstacle();

};
