// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class PROTOTYPE0UNREAL_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyHealthBarWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyDamageEnemy();

	UFUNCTION(BlueprintImplementableEvent)
	void ActivateEnemy();

	UFUNCTION(BlueprintImplementableEvent)
	void DeactivateEnemy();
public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float MaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float currentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool isAttacking;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	virtual void TakeDamage(float distance, float damage, FVector sourcePos, float launchForce);
	
	UFUNCTION(BlueprintCallable)
	bool IsInAttackRange(AActor* targetToAttack);
};
