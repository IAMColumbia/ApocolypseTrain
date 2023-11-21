// Fill out your copyright notice in the Description page of Project Settings.


#include "Sniper.h"
#include "Obstacle.h"
#include "EnemyCharacter.h"
#include "MyCharacter.h"

void ASniper::Ray()
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
	TArray<FHitResult> hits;

	if (GetWorld()) {
		FCollisionObjectQueryParams objectParams;
		objectParams.AddObjectTypesToQuery(ECC_Pawn);
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(OwnerCharacter);
		bool actorHit = GetWorld()->LineTraceMultiByObjectType(hits, start, end, objectParams.AllObjects, QueryParams);
		int enemiesHit = 1;
		for (int i = 0; i < hits.Num(); i++) {
			//DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.15f, 0.f, 10.f);
			if (actorHit && hits[i].GetActor()) {
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, hits[i].GetActor()->GetFName().ToString());
				if (AEnemyCharacter* enemy = Cast<AEnemyCharacter>(hits[i].GetActor())) {
					enemy->TakeDamage(hits[i].Distance, Damage - FMath::RandRange(0, ((int)(OwnerCharacter->DamageBuff / 2) + 3)) + OwnerCharacter->DamageBuff, GetActorLocation(), KnockbackForce/enemiesHit);
					enemiesHit++;
				}
				if (AObstacle* obstacle = Cast<AObstacle>(hits[i].GetActor())) {
					obstacle->DamageObstacle(Damage);
					enemiesHit++;
				}
			}
			if (enemiesHit > Penetration) {
				return;
			}
		}
		

	}
}
