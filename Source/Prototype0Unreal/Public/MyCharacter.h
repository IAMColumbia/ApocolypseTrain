// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UENUM(BlueprintType)
enum class EUpgradeType : uint8{damage UMETA(DisplayName="Damage"), health UMETA(DisplayName = "Health"), speed UMETA(DisplayName = "Speed")
};

UCLASS()
class PROTOTYPE0UNREAL_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()



public:
	// Sets default values for this character's properties
	AMyCharacter();

	bool IsFacingWall();

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

	//how much damage players do to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Damage;

	//the rate at which players passivly increase health if below max
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float RegenRate;

	//how much is added to players base damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buffs")
	float DamageBuff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buffs")
	float SpeedBuff;

	float baseHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool CanAddFuel;

	FTimerHandle shootTimerHandle;
	FTimerHandle respawnTimerHandle;

	//How long it takes the player to respawn when they die
	UPROPERTY(EditAnywhere, Category = "Respawning")
	int TotalRespawnTime;
	int currentRespawnTime;

	UFUNCTION(BlueprintCallable, Category = "Respawning")
	void UpdateRespawnTimer();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	void setRotation();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float RayOffset = 80;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float RayLength = 1800;
	void Ray();

	void ShootPressed();
	void ShootReleased();

	

	void InteractPressed();
	void InteractReleased();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyHealthBarWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyStartedShooting();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyFiredShot(FVector direction);

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyStoppedShooting();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyPlayerDied();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyPlayerRespawn();

public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool Interacted;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool Carrying;

	class AInteractableActor* carriedObject;

	USceneComponent* CarryPosition;

	void PickupItem(class AInteractableActor* itemToCarry);

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

	UFUNCTION(BlueprintCallable, Category = "Respawning")
	FVector GetRespawnLocation();

	//Player health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool IsPlayerDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float currentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Initialize")
	int PlayerIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Initialize")
	FVector PlayerColor;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TakeDamage(float damageToTake);

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyTakeDamage();

	UFUNCTION(BlueprintCallable, Category = "Initialize")
	void OnPlayerSpawn();

	FColor GetPlayerColor();

	FVector SetPlayerColorVector(int index);

	bool HasFuel();

	UFUNCTION(BlueprintImplementableEvent)
	void ApplyUpgrade(EUpgradeType upgrade,float amount);

	UFUNCTION(BlueprintCallable)
	void Heal();

private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	TSubclassOf<AActor> ActorToSpawn;

	void DespawnPlayer();

	void RegenerateHealth();

};
