// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Weapon.h"

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveProjectile();
}

void AProjectile::Spawn() {
	SetActorLocation(Owner->BulletSpawn->GetComponentLocation());
	SetActorRotation(Owner->BulletSpawn->GetComponentRotation());
	APooledActor::Spawn();
	GetWorld()->GetTimerManager().SetTimer(lifetimeHandle, this, &APooledActor::Despawn, Lifetime, false);
}

void AProjectile::Despawn()
{
	APooledActor::Despawn();
}

void AProjectile::InitializeProjectile(AWeapon* owner)
{
	Owner = owner;
}

void AProjectile::MoveProjectile()
{
	FVector location = GetActorLocation();
	location += GetActorForwardVector() * ProjectileSpeed;
	SetActorLocation(location);
}
