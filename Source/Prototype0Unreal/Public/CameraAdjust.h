// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraAdjust.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOTYPE0UNREAL_API UCameraAdjust : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraAdjust();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	class UPlayerManagerWSS* playerManager;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TrainPositionOffset;

	FVector curLoc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float cameraSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float deadZone;
	float smoothOffset;
	float GetYDir();
};
