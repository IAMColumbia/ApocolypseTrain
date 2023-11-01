// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include <GameFramework/Actor.h>
#include "GameManagerWSS.h"
#include <Kismet/KismetMathLibrary.h>
#include "MyCharacter.h"

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
	if (GetWorld()->GetSubsystem<UGameManagerWSS>()->IsOutOfBackBounds(GetActorLocation())) {
		Destroy();
	}
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::TakeDamage(float distance, float damage, FVector sourcePos, float launchForce) {
	if (EnemyState == EEnemyState::Dead) {
		return;
	}
	currentHealth -= damage;
	NotifyHealthBarWidget();
	NotifyDamageEnemy();
	LaunchCharacter((GetActorLocation()-sourcePos) * launchForce,false, false);
	if (currentHealth <= 0) {
		EnemyState = EEnemyState::Dead;
		EnemyKilled();
	}
}

bool AEnemyCharacter::IsInAttackRange(AActor* targetToAttack) {
	if (targetToAttack == NULL) {
		UE_LOG(LogTemp, Warning, TEXT("NULL TARGET FOR ENEMEMY"));
		return false;
	}
	double distance = (GetActorLocation() - targetToAttack->GetActorLocation()).Size();
	if (distance < AttackRange) {
		if (AMyCharacter* targ = Cast<AMyCharacter>(targetToAttack)) {
			if (targ->IsPlayerDead) {
				return false;
			}
		}
		return true;
	}
	else {
		return false;
	}
}



