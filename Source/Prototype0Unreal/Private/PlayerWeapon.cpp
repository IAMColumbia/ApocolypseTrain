// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWeapon.h"
#include "MyCharacter.h"
#include "EnemyCharacter.h"
#include "Obstacle.h"

// Sets default values for this component's properties
UPlayerWeapon::UPlayerWeapon()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UPlayerWeapon::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = GetOwner<AMyCharacter>();
	if (OwnerCharacter != NULL) {
		GEngine->AddOnScreenDebugMessage(-1, 6, OwnerCharacter->GetPlayerColor(), TEXT("Player Registered!!!"));
	}
}


// Called every frame
void UPlayerWeapon::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerWeapon::Attack(FColor color)
{
	GEngine->AddOnScreenDebugMessage(-1, 3, color, TEXT("WeaponAttacked"));
	Ray();
}

void UPlayerWeapon::Ray()
{
	FVector start = OwnerCharacter->GetActorLocation();

	//FVector forward = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(characterMesh->GetRightVector(), 0.8);
	FVector forward = OwnerCharacter->characterMesh->GetRightVector();
	forward.Z = 0;

	start = FVector(start.X + (forward.X * OwnerCharacter->RayOffset), start.Y + (forward.Y * OwnerCharacter->RayOffset), start.Z + (forward.Z * OwnerCharacter->RayOffset));
	//maybe need to change end pos for randomness
	FVector end = start + forward * OwnerCharacter->RayLength;

	FHitResult hit;


	if (GetWorld()) {
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(OwnerCharacter);
		bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Pawn, QueryParams, FCollisionResponseParams());
		OwnerCharacter->NotifyFiredShot(OwnerCharacter->GetActorForwardVector());
		//DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.15f, 0.f, 10.f);
		if (actorHit && hit.GetActor()) {
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, hit.GetActor()->GetFName().ToString());
			if (AEnemyCharacter* enemy = Cast<AEnemyCharacter>(hit.GetActor())) {
				enemy->TakeDamage(hit.Distance, 10- FMath::RandRange(0, ((int)(2 / 2) + 3)) + 2, OwnerCharacter->GetActorLocation(), 2);
			}
			if (AObstacle* obstacle = Cast<AObstacle>(hit.GetActor())) {
				obstacle->DamageObstacle(10);
			}
		}

	}
}

