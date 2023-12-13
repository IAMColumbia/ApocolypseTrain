// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class PROTOTYPE0UNREAL_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	class AMyCharacter* OwnerCharacter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FTimerHandle shootTimerHandle;
	FTimerHandle reloadTimerHandle;

	virtual void ShootProjectile();

	USceneComponent* weaponRotator;

public:	

	void KilledEnemy();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetBeamEnd();

	USceneComponent* BulletSpawn;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Attack();
	virtual void EndAttack();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RayOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RayLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KnockbackForce;

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyFiredShot();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Audio")
	USoundBase* equipSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Audio")
	USoundAttenuation* attenuation;

	void WeaponEquipped();

	virtual void Ray();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int OwnerPlayerIndex();

	float currentReloadTime;

	void UpdateReloadTime();
	void CheckForAttack();
	

	bool Reloaded;
	bool Attacking;
	void Reload();

	USceneComponent* laser;

	void HideLaser();

	void ShowLaser();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<class AProjectile> ProjectileType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	int PoolCount;

	TQueue<class AProjectile*> objectPool;
	void CreateObjects();
	virtual void SpawnProjectile();

	FQuat OriginalRotation;
	bool upright;
	bool Clipping();
};
