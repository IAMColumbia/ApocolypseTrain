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
	class UGameManagerWSS* gameManager;
	class ATrain* trainPtr;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool CanAddFuel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fuel")
	int Fuel;

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

	void InteractPressed();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyHealthBarWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyFuelDisplay();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool IsPlayerDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float currentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Initialize")
	float BackBoundOffsetFromTrain;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Initialize")
	int PlayerIndex;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TakeDamage(float damageToTake);

	UFUNCTION(BlueprintCallable, Category = "Initialize")
	void OnPlayerSpawn();

	


private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	TSubclassOf<AActor> ActorToSpawn;


};
