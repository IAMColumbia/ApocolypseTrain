// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableActor.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StatUpgrade.generated.h"

enum class EUpgradeType : uint8;
class AMyCharacter;

UCLASS()
class PROTOTYPE0UNREAL_API AStatUpgrade : public AInteractableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStatUpgrade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void CheckForInteractPressed() override;

	virtual void OnInteract(AMyCharacter* player) override;

	//FText getUpgradeText(float value, EUpgradeType type);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float OpenPercent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float OpenRate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float OpenCompletePercent;


	UFUNCTION(BlueprintImplementableEvent)
	void Interacted(const FText &textToDisplay, FVector color);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector ProgressColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DefaultColor;

	AMyCharacter* activePlayer;

};
