// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class PROTOTYPE0UNREAL_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()



public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	float xRot;
	float yRot;
	void setXRot(float AxisValue);
	void setYRot(float AxisValue);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UWeapon* PlayerWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Combat")
	bool IsShooting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Damage;

	FTimerHandle shootTimerHandle;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	void setRotation();

	float RayOffset = 80;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	float RayLength = 1800;
	void Ray();

	void ShootPressed();
	void ShootReleased();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyHealthBarWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyStartedShooting(FVector direction);

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyStoppedShooting();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyPlayerDied();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Mesh")
	USkeletalMeshComponent* characterMesh;

	UFUNCTION(BlueprintCallable, Category = "ProjectileSpawning")
	void ShootProjectile();

	UFUNCTION(BlueprintCallable, Category = "Respawning")
	void ResetPlayer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float MaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float currentHealth;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TakeDamage(float damageToTake);
private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	TSubclassOf<AActor> ActorToSpawn;


};
