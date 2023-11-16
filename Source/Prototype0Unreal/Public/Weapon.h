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

	virtual void ShootProjectile();

	USceneComponent* BulletSpawn;

public:	
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

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyFiredShot(FVector direction);

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyAttackStart();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyAttackEnd();

	void Ray();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int OwnerPlayerIndex();
};
