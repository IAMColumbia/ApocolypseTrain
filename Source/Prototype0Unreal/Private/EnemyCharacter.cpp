// Fill out your copyright notice in the Description page of Project Settings.

#include <GameFramework/Actor.h>
#include "EnemyCharacter.h"
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

void AEnemyCharacter::AttackTarget(AActor* target) {
	double distance = (GetActorLocation() - target->GetActorLocation()).Size();
	if (distance < AttackRange) {
		//isAttacking = true;
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::MakeRandomColor(), FString::Printf(TEXT("attackrnage = %d"), distance));
	}
	else {
		//maybe use an anim notify to call this method again once the attack animation is done and see if the enemy is still near the player
		//isAttacking = false;
	}
}


