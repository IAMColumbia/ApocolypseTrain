// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableActor.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LootBox.generated.h"

enum class EUpgradeType : uint8;
class AMyCharacter;

UCLASS()
class PROTOTYPE0UNREAL_API ALootBox : public AInteractableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALootBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void CheckForInteractPressed() override;

	virtual void OnInteract(AMyCharacter* player) override;

	//FText getUpgradeText(float value, EUpgradeType type);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float OpenPercent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float OpenRate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float OpenCompletePercent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float UpgradeAmount;

	UFUNCTION(BlueprintImplementableEvent)
	void Interacted(const FText &textToDisplay, FVector color);

	UFUNCTION(BlueprintImplementableEvent)
	void BeginInteract();

	bool startedInteract;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector ProgressColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DefaultColor;

	AMyCharacter* activePlayer;

};
