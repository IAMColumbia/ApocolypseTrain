// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveProjectile.h"
#include "Components/SphereComponent.h"

// Sets default values
AExplosiveProjectile::AExplosiveProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AExplosiveProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplosiveProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplosiveProjectile::Explode()
{
	float radius = 200;
	USphereComponent* explosionHitCheck = Cast<USphereComponent>(GetWorld()->SpawnActorAbsolute(USphereComponent::StaticClass(), GetActorTransform()));
	explosionHitCheck->InitSphereRadius(radius);
}



