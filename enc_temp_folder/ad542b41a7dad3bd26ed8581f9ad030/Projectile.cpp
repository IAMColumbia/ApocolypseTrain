// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Weapon.h"

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveProjectile();
}

void AProjectile::Spawn() {
	SetActorRotation(spawnPoint->GetComponentRotation());
	SetActorLocation(spawnPoint->GetComponentLocation());
	APooledActor::Spawn();
	GetWorld()->GetTimerManager().SetTimer(lifetimeHandle, this, &APooledActor::Despawn, Lifetime, false);
	Active = true;
}

void AProjectile::Despawn()
{
	Active = false;
	APooledActor::Despawn();
}

void AProjectile::InitializeProjectile(AWeapon* owner)
{
	Owner = owner;
	spawnPoint = Owner->BulletSpawn;
}

bool AProjectile::ShouldDespawn()
{
	return false;
}



void AProjectile::MoveProjectile()
{
	FVector location = GetActorLocation();
	location += GetActorForwardVector() * ProjectileSpeed;
	SetActorLocation(location);
}
