// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScrollActor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOTYPE0UNREAL_API UScrollActor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UScrollActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float scrollSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	USceneComponent* root;

	FVector currentLocation;
protected:

	
	// Called when the game starts
	virtual void BeginPlay() override;

	void Scroll(float DeltaTime);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
