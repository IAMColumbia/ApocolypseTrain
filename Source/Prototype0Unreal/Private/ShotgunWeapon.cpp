// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunWeapon.h"
#include "MyCharacter.h"
#include "Obstacle.h"
#include "EnemyCharacter.h"
#include "Projectile.h"
#include <Kismet/KismetMathLibrary.h>

void AShotgunWeapon::Ray()
{
	for (int i = 0; i < raysPerShot; i++) {
		if (BulletSpawn == NULL) {
			return;
		}
		FVector start = GetActorLocation();

		FVector forward = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(BulletSpawn->GetForwardVector(), shotArc);
		//FVector forward = OwnerCharacter->characterMesh->GetRightVector();
		//FVector forward = BulletSpawn->GetForwardVector();
		forward.Z = 0;

		start = FVector(start.X + (forward.X * RayOffset), start.Y + (forward.Y * RayOffset), start.Z + (forward.Z * RayOffset));
		//maybe need to change end pos for randomness
		FVector end = start + forward * RayLength;

		FHitResult hit;

		float yaw;
		float pitch;
		UKismetMathLibrary::GetYawPitchFromVector(forward, yaw, pitch);
		SpawnShotCosmetic(FRotator(pitch,yaw,0));


		if (GetWorld()) {
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);
			QueryParams.AddIgnoredActor(OwnerCharacter);
			bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Pawn, QueryParams, FCollisionResponseParams());
			//DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.15f, 0.f, 10.f);
			if (actorHit && hit.GetActor()) {
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, hit.GetActor()->GetFName().ToString());
				if (AEnemyCharacter* enemy = Cast<AEnemyCharacter>(hit.GetActor())) {
					if (enemy->TakeDamage(hit.Distance, Damage - FMath::RandRange(0, ((int)(OwnerCharacter->DamageBuff / 2) + 3)) + OwnerCharacter->DamageBuff, GetActorLocation(), KnockbackForce)) {
						KilledEnemy();
					}
				}
				if (AObstacle* obstacle = Cast<AObstacle>(hit.GetActor())) {
					obstacle->DamageObstacle(Damage);
				}
			}

		}
	
	}
}

void AShotgunWeapon::SpawnProjectile()
{
	
}

void AShotgunWeapon::SpawnShotCosmetic(FRotator rot)
{
	if (!objectPool.IsEmpty()) {
		AProjectile* p;
		objectPool.Dequeue(p);
		p->SpawnWithRotation(rot);
		objectPool.Enqueue(p);
	}
}
