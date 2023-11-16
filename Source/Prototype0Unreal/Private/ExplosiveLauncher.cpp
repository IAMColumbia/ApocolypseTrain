// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveLauncher.h"
#include "MyCharacter.h"
#include "ExplosiveProjectile.h"



void AExplosiveLauncher::BeginPlay()
{
	Super::BeginPlay();
}

void AExplosiveLauncher::ShootProjectile()
{
	FVector LaunchDirection = OwnerCharacter->GetActorForwardVector();
	AExplosiveProjectile* a = Cast<AExplosiveProjectile>(GetWorld()->SpawnActor(ExplosiveProjectile));

}
