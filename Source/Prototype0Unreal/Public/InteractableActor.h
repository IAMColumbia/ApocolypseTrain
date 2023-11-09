// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

class AMyCharacter;

UENUM()
enum class EInteractableState : uint8 { Idle UMETA(DisplayName = "Idle"), Carried UMETA(DisplayName = "Carried"), Dropped UMETA(DisplayName = "Dropped")
};

UCLASS()
class PROTOTYPE0UNREAL_API AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EInteractableState state;

	// Sets default values for this actor's properties
	AInteractableActor();

	TArray<AMyCharacter* > overlappingPlayers;


	class USphereComponent* trigger;

	void DropObject(FVector directionToLaunch);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CheckForInteractPressed();

	virtual void OnInteract(AMyCharacter* player);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//make sure overrided events are marked as UFUNCTION
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPickedUp(USceneComponent* carrier);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDropped(FVector directionToLaunch);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayersOverlapping();

	UFUNCTION(BlueprintImplementableEvent)
	void NoPlayersOverlapping();
};
