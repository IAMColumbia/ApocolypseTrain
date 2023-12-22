// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class UPlayerWeapon;
class AWeapon;

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

	//how much is added to players base damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buffs")
	float DamageBuff;

	TSubclassOf<AWeapon> PickupWeapon(TSubclassOf<AWeapon> weaponToPickup);

	TSubclassOf<AWeapon> GetCurrentWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int TotalKills;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int TotalDeaths;

protected:
	void AttachWeapon();
	void HolsterWeapon();

	class UGameManagerWSS* gameManager;
	class ATrain* trainPtr;

	float xRot;
	float yRot;
	void setXRot(float AxisValue);
	void setYRot(float AxisValue);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class AWeapon* CurrentWeapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AWeapon> DefaultWeapon;

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

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buffs")
	float SpeedBuff;

	float baseHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool CanAddFuel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float KnockBackDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	bool isDashing;

	bool canDash;

	float DashSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float DashDecelleration;
	void ApplyDash();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyOnDash(FVector direction);

	UFUNCTION(BLueprintImplementableEvent)
	void OnDashFinish();

	//UFUNCTION();
	void ResetDash();
	void FinishDash();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector DashDirection;

	FTimerHandle dashCooldownTimerHandle;
	FTimerHandle dashTimerHandle;

	FTimerHandle shootTimerHandle;
	FTimerHandle respawnTimerHandle;

	bool Invincible;
	void SetInvincibility();

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

	void ShootPressed();
	void ShootReleased();

	void DashPressed();

	void InteractPressed();
	void InteractReleased();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyHealthBarWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyStartedShooting();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyStoppedShooting();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyPlayerDied();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyPlayerRespawn();

	UFUNCTION(BlueprintImplementableEvent)
	void SetMovementSpeed(float speed);

public:	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool isWeaponUpright();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool Interacted;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool Carrying;

	bool justDropped;

	class AInteractableActor* carriedObject;

	USceneComponent* CarrySlot;
	USceneComponent* HolsterSlot;

	void PickupItem(class AInteractableActor* itemToCarry);

	void CheckDropItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Mesh")
	USkeletalMeshComponent* characterMesh;

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

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyReloadPercent(float currentTime, float MaxTime);

	bool overlappingInteractable;
private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	TSubclassOf<AActor> ActorToSpawn;

	void DespawnPlayer();

	void RegenerateHealth();

};
