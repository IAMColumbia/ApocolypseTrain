// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PooledActor.h"
#include "Projectile.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE0UNREAL_API AProjectile : public APooledActor
{
	GENERATED_BODY()
	
public:

	virtual void Tick(float DeltaTime) override;

	virtual void Spawn() override;
	UFUNCTION(BlueprintCallable)
	virtual void Despawn() override;
	virtual void InitializeProjectile(class AWeapon* owner);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Lifetime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ProjectileSpeed;
	
	bool Active;

	

protected:
	class AWeapon* Owner;
	USceneComponent* spawnPoint;
	FTimerHandle lifetimeHandle;

	void MoveProjectile();
};
