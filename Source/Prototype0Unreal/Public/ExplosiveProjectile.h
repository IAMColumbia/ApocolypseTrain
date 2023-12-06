// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosiveProjectile.generated.h"

UCLASS()
class PROTOTYPE0UNREAL_API AExplosiveProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosiveProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	class AWeapon* ownerWeapon;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void KilledEnemy();

	UPROPERTY(BlueprintReadOnly)
	int FiredPlayerIndex;

	UFUNCTION(BlueprintImplementableEvent)
	void Launch(FVector Direction);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Velocity;

	UFUNCTION(BlueprintCallable)
	void Explode();
};
