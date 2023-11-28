// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveLauncher.h"
#include "MyCharacter.h"
#include "ExplosiveProjectile.h"



void AExplosiveLauncher::BeginPlay()
{
	Super::BeginPlay();
	FireRate = 1;
}

void AExplosiveLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AExplosiveLauncher::ShootProjectile()
{
	FVector LaunchDirection = BulletSpawn->GetForwardVector();
	AExplosiveProjectile* a = Cast<AExplosiveProjectile>(GetWorld()->SpawnActorAbsolute(ExplosiveProjectile, BulletSpawn->GetComponentTransform()));
	a->Launch(LaunchDirection);
	a->FiredPlayerIndex = OwnerPlayerIndex();
}
