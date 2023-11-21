// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "MyCharacter.h"
#include "EnemyCharacter.h"
#include "Obstacle.h"
#include "Projectile.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	TSet<UActorComponent*> components = GetComponents();
	for (UActorComponent* component : components) {
		if (component->IsA(USceneComponent::StaticClass())) {
			USceneComponent* sceneComponent = Cast<USceneComponent>(component);
			if (sceneComponent && sceneComponent->ComponentHasTag("BulletSpawn")) {
				BulletSpawn = sceneComponent;
			}
		}
	}
	Reload();
	CreateObjects();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateReloadTime();
	CheckForAttack();
}

void AWeapon::Ray()
{
	if (BulletSpawn == NULL) {
		return;
	}
	FVector start = GetActorLocation();

	//FVector forward = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(characterMesh->GetRightVector(), 0.8);
	//FVector forward = OwnerCharacter->characterMesh->GetRightVector();
	FVector forward = BulletSpawn->GetForwardVector();
	forward.Z = 0;

	start = FVector(start.X + (forward.X * RayOffset), start.Y + (forward.Y * RayOffset), start.Z + (forward.Z * RayOffset));
	//maybe need to change end pos for randomness
	FVector end = start + forward * RayLength;

	FHitResult hit;


	if (GetWorld()) {
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(OwnerCharacter);
		bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Pawn, QueryParams, FCollisionResponseParams());
		//NotifyFiredShot(OwnerCharacter->GetActorRightVector());
		
		//DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.15f, 0.f, 10.f);
		if (actorHit && hit.GetActor()) {
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, hit.GetActor()->GetFName().ToString());
			if (AEnemyCharacter* enemy = Cast<AEnemyCharacter>(hit.GetActor())) {
				enemy->TakeDamage(hit.Distance, Damage - FMath::RandRange(0, ((int)(OwnerCharacter->DamageBuff / 2) + 3)) + OwnerCharacter->DamageBuff, GetActorLocation(), KnockbackForce);
			}
			if (AObstacle* obstacle = Cast<AObstacle>(hit.GetActor())) {
				obstacle->DamageObstacle(Damage);
			}
		}

	}
}

int AWeapon::OwnerPlayerIndex()
{
	return OwnerCharacter->PlayerIndex;
}

void AWeapon::UpdateReloadTime()
{
	currentReloadTime = GetWorld()->GetTimerManager().GetTimerElapsed(reloadTimerHandle);
	OwnerCharacter->NotifyReloadPercent(currentReloadTime, FireRate);
}

void AWeapon::CheckForAttack()
{
	if (Attacking && Reloaded) {
		if (Reloaded) {
			NotifyFiredShot();
			ShootProjectile();
			Reloaded = false;
			GetWorld()->GetTimerManager().SetTimer(reloadTimerHandle, this, &AWeapon::Reload, FireRate, false);
		}
	}
}

void AWeapon::Reload()
{
	Reloaded = true;
}

void AWeapon::CreateObjects()
{
	for (int i = 0; i < PoolCount; i++) {
		AProjectile* p = Cast<AProjectile>(GetWorld()->SpawnActor(ProjectileType));
		if (p != NULL) {
			p->InitializeProjectile(this);
			p->Despawn();
			objectPool.Enqueue(p);
		}
	}
}

void AWeapon::SpawnProjectile()
{
	if (!objectPool.IsEmpty()) {
		AProjectile* p;
		objectPool.Dequeue(p);
		p->Spawn();
		objectPool.Enqueue(p);
	}
}


void AWeapon::Attack()
{
	Attacking = true;
}

void AWeapon::ShootProjectile() {
	SpawnProjectile();
	Ray();
}

void AWeapon::EndAttack()
{
	Attacking = false;
}

