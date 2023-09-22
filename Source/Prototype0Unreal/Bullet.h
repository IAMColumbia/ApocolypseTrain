// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class PROTOTYPE0UNREAL_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet")
	class USceneComponent* RootScene;
	// Sets default values for this actor's properties
	ABullet();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Bullet")
	float speed = 10;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
