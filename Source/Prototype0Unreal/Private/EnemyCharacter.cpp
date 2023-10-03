// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include <GameFramework/Actor.h>
#include <Kismet/KismetMathLibrary.h>

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	currentHealth = MaxHealth;
	NotifyHealthBarWidget();
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::TakeDamage(float distance, float damage) {
	currentHealth -= damage;
	NotifyHealthBarWidget();
	if (currentHealth <= 0) {
		Destroy();
	}
}

bool AEnemyCharacter::IsInAttackRange(AActor* target) {
	double distance = (GetActorLocation() - target->GetActorLocation()).Size();
	if (distance < AttackRange) {
		return true;
	}
	else {
		return false;
	}
}


