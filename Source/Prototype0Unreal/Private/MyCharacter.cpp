// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetSystemLibrary.h>
#include <EnemyCharacter.h>
#include "GameManagerWSS.h"
#include "Train.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerManagerWSS.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	characterMesh = FindComponentByClass<USkeletalMeshComponent>();
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	currentHealth = MaxHealth;
	NotifyHealthBarWidget();
	trainPtr = GetWorld()->GetSubsystem<UGameManagerWSS>()->train;
	
}

void AMyCharacter::OnPlayerSpawn() {
	PlayerIndex = UGameplayStatics::GetPlayerControllerID(((APlayerController*)GetController()));
	PlayerColor = SetPlayerColorVector(PlayerIndex);
	GEngine->AddOnScreenDebugMessage(-1, 5, GetPlayerColor(), FString::Printf(TEXT("Player With Index %d Joined The Game"), PlayerIndex));
	SetActorLocation(trainPtr->GetRespawnPos(PlayerIndex));
	GetWorld()->GetSubsystem<UPlayerManagerWSS>()->RegisterPlayer(this);
}


// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AMyCharacter::setXRot(GetInputAxisValue("Horizontal"));
	AMyCharacter::setYRot(GetInputAxisValue("Vertical"));
	AMyCharacter::setRotation();
	if (trainPtr->IsOverlappingFuelBox(GetActorLocation())) {
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Orange, FString::Printf(TEXT("is in box")));
		CanAddFuel = true;
	}
	else {
		CanAddFuel = false;
	}
	DrawDebugBox(GetWorld(), trainPtr->GetRespawnPos(PlayerIndex), FVector(1, 1, 1) * 60, GetPlayerColor(), false, -1.0f, 0U, 10.0f);
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("ShootWeapon", EInputEvent::IE_Pressed, this, &AMyCharacter::ShootPressed);
	PlayerInputComponent->BindAction("ShootWeapon", EInputEvent::IE_Released, this, &AMyCharacter::ShootReleased);
	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &AMyCharacter::InteractPressed);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
	
}

void AMyCharacter::InteractPressed() {
	if (trainPtr->IsOverlappingFuelBox(GetActorLocation()) && Fuel > 0) {
		if (trainPtr->AddFuel()) {
			Fuel--;
		}
		NotifyFuelDisplay();
	}
}

void AMyCharacter::setXRot(float AxisValue) {
	if (AxisValue != 0.0f) {
		xRot = AxisValue * -1;
	}
}

void AMyCharacter::setYRot(float AxisValue) {
	if (AxisValue != 0.0f) {
		yRot = AxisValue;
	}
}

void AMyCharacter::ShootPressed() {
	IsShooting = true;
	NotifyStartedShooting(GetActorForwardVector());
	shootTimerHandle = UKismetSystemLibrary::K2_SetTimer(this, TEXT("ShootProjectile"), FireRate, true, 0, 0);
}

void AMyCharacter::ShootReleased() {
	IsShooting = false;
	NotifyStoppedShooting();
	UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(this, shootTimerHandle);
}

void AMyCharacter::ShootProjectile() {
	Ray();
	
	//GetWorld()->SpawnActor<AActor>(ActorToSpawn, GetActorLocation(), characterMesh->GetComponentRotation() + FRotator(0,90,0));
}

void AMyCharacter::MoveForward(float AxisValue) {
	if ((Controller != NULL) && AxisValue != 0.0f) {
		//find out which direction is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, AxisValue);
	}
}

void AMyCharacter::MoveRight(float AxisValue) {
	if (Controller != NULL && AxisValue != 0.0f) {

		//find out which direction is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		//add movement in that direction
		AddMovementInput(Direction, AxisValue);
	}
}

void AMyCharacter::setRotation() {
	FVector dir = FVector(xRot, yRot, 0); //UKismetMathLibrary::MakeVector(xRot, yRot, 0);
	FRotator rotation = UKismetMathLibrary::MakeRotationFromAxes(dir, FVector::Zero(), FVector::Zero());
	characterMesh->SetWorldRotation(rotation);
}

void AMyCharacter::Ray()
{
	FVector start = GetActorLocation();
	//getting right as the forward vector is offset 90 degrees
	FVector forward = characterMesh->GetRightVector();
	start = FVector(start.X + (forward.X * RayOffset), start.Y + (forward.Y * RayOffset), start.Z + (forward.Z * RayOffset));
	FVector end = start + (forward * RayLength);

	FHitResult hit;

	
	if (GetWorld()) {
		bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Pawn, FCollisionQueryParams(), FCollisionResponseParams());
		//DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.15f, 0.f, 10.f);
		if (actorHit && hit.GetActor()) {
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, hit.GetActor()->GetFName().ToString());
			if (AEnemyCharacter* enemy = Cast<AEnemyCharacter>(hit.GetActor())) {
				enemy->TakeDamage(hit.Distance, Damage);
			}
		}
		
		//if(actorHit && hit.GetActor() == Enemy)
		//need to insert a cpp class and make enemy ai inherit from that class
		//make a method in cpp for taking damage
		//override or call that method from enemy BP
		//maybe pass in the distance from player to determine a delay before the enemy starts taking damage
	}
}

void AMyCharacter::TakeDamage(float damageToTake) {
	currentHealth -= damageToTake;
	//GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::MakeRandomColor(), FString::Printf(TEXT("PLAYER WAS HIT for %f damage"), damageToTake));
	NotifyHealthBarWidget();
	if (currentHealth <= 0) {
		IsPlayerDead = true;
		if (IsShooting) {
			ShootReleased();
		}
		NotifyPlayerDied();
	}
}

void AMyCharacter::OnPlayerDeath() {

}

FColor AMyCharacter::GetPlayerColor()
{
	return FColor(PlayerColor.X * 255, PlayerColor.Y * 255, PlayerColor.Z * 255, 255);
}

FVector AMyCharacter::SetPlayerColorVector(int index)
{
	switch (index) {
	case 0:
		return FVector(0, 0.4, 1);
	case 1:
		return FVector(0.4, 1, 0);
	case 2:
		return FVector(1, 0.5 , 0 );
	case 3:
		return FVector(0 , 0.5 , 0.5 );
	default:
		return FVector(255, 255, 255);
	}
}

void AMyCharacter::ResetPlayer() {
	SetActorLocation(trainPtr->GetRespawnPos(PlayerIndex));
	currentHealth = MaxHealth;
	IsPlayerDead = false;
	NotifyHealthBarWidget();
}



