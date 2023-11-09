// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UENUM()
enum class EEnemyState : uint8 {
	Idle UMETA(DisplayName = "Idle"), Dead UMETA(DisplayName = "Dead")
};

UCLASS()
class PROTOTYPE0UNREAL_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EEnemyState EnemyState;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyHealthBarWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyDamageEnemy(float Damage);

	UFUNCTION(BlueprintImplementableEvent)
	void ActivateEnemy();

	UFUNCTION(BlueprintImplementableEvent)
	void DeactivateEnemy();

	UFUNCTION(BlueprintImplementableEvent)
	void EnemyKilled();
public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float MaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	float currentHealth;
	//how far away from the player before the enemy starts playing the attack animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool isAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	float enemyHeight;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float distance, float damage, FVector sourcePos, float launchForce);
	
	UFUNCTION(BlueprintCallable)
	bool IsInAttackRange(AActor* targetToAttack);
};
