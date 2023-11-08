// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include <EnemyCharacter.h>
#include "GameManagerWSS.h"
#include "Train.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerManagerWSS.h"
#include "Obstacle.h"
#include <InteractableActor.h>

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	characterMesh = FindComponentByClass<USkeletalMeshComponent>();
}

bool AMyCharacter::IsFacingWall()
{
	FVector start = GetActorLocation();

	FVector forward = characterMesh->GetRightVector();
	forward.Z = 0;

	start = FVector(start.X + (forward.X), start.Y + (forward.Y), start.Z + (forward.Z));
	//maybe need to change end pos for randomness
	FVector end = start + forward * 80;

	FHitResult hit;

	if (GetWorld()) {
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(carriedObject);
		bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_WorldDynamic,QueryParams, FCollisionResponseParams());
		//DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.15f, 0.f, 10.f);
		if (actorHit && hit.GetActor()) {
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, hit.GetActor()->GetFName().ToString());
			if (AEnemyCharacter* enemy = Cast<AEnemyCharacter>(hit.GetActor())) {
				return false;
			}
			if (AObstacle* obstacle = Cast<AObstacle>(hit.GetActor())) {
				return false;
			}
			return true;
		}
	}
	return false;
}


// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	currentHealth = MaxHealth;
	NotifyHealthBarWidget();
	gameManager = GetWorld()->GetSubsystem<UGameManagerWSS>();
	trainPtr = gameManager->train;

	TSet<UActorComponent*> components = GetComponents();
	for (UActorComponent* component : components) {
		if (component->IsA(USceneComponent::StaticClass())) {
			USceneComponent* sceneComponent = Cast<USceneComponent>(component);
			if (sceneComponent && sceneComponent->ComponentHasTag("PickupSlot")) {
				CarryPosition = sceneComponent;
			}
		}
	}
}

void AMyCharacter::OnPlayerSpawn() {
	PlayerIndex = UGameplayStatics::GetPlayerControllerID(((APlayerController*)GetController()));
	PlayerColor = SetPlayerColorVector(PlayerIndex);
	GEngine->AddOnScreenDebugMessage(-1, 5, GetPlayerColor(), FString::Printf(TEXT("Player With Index %d Joined The Game"), PlayerIndex));
	SetActorLocation(trainPtr->GetRespawnPos(PlayerIndex));
	GetWorld()->GetSubsystem<UPlayerManagerWSS>()->RegisterPlayer(this);
	trainPtr->OnControllerPosses(PlayerIndex, GetPlayerColor());
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
	//DrawDebugBox(GetWorld(), trainPtr->GetRespawnPos(PlayerIndex), FVector(1, 1, 1) * 60, GetPlayerColor(), false, -1.0f, 0U, 10.0f);
	if (gameManager->IsOutOfBounds(GetActorLocation())) {
		DespawnPlayer();
	}
	RegenerateHealth();
}

#pragma region Movement

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("ShootWeapon", EInputEvent::IE_Pressed, this, &AMyCharacter::ShootPressed);
	PlayerInputComponent->BindAction("ShootWeapon", EInputEvent::IE_Released, this, &AMyCharacter::ShootReleased);
	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &AMyCharacter::InteractPressed);
	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Released, this, &AMyCharacter::InteractReleased);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
}

void AMyCharacter::InteractPressed() {
	Interacted = true;
	if (trainPtr->IsOverlappingFuelBox(GetActorLocation()) && Carrying) {
		if (carriedObject != NULL) {
			if (trainPtr->AddFuel()) {
				carriedObject->Destroy();
			}
		}
	}
	if (trainPtr->IsOverlappingLeverBox(GetActorLocation()) && !Carrying) {
		trainPtr->ToggleTrainState();
	}
	if (Carrying) {
		if (carriedObject != NULL ) {
			if (IsFacingWall()) {
				carriedObject->DropObject(characterMesh->GetRightVector()*-1);
			}
			else {
				carriedObject->DropObject(characterMesh->GetRightVector());
			}
		}
		Carrying = false;
	}
}

void AMyCharacter::InteractReleased()
{
	Interacted = false;
}

void AMyCharacter::PickupItem(AInteractableActor* itemToCarry)
{
	if (Carrying) {
		return;
	}
	Carrying = true;
	carriedObject = itemToCarry;
	itemToCarry->OnPickedUp(CarryPosition);
	ShootReleased();
	//itemToCarry->SetActorLocation(CarryPosition->GetComponentLocation());
	//itemToCarry->AttachToComponent(CarryPosition, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
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

#pragma endregion Movement

#pragma region Shooting

void AMyCharacter::ShootPressed() {
	if (Carrying) {
		return;
	}
	IsShooting = true;
	NotifyStartedShooting();
	ShootProjectile();
	GetWorld()->GetTimerManager().SetTimer(shootTimerHandle, this, &AMyCharacter::ShootProjectile, FireRate, true);
}

void AMyCharacter::ShootReleased() {
	IsShooting = false;
	NotifyStoppedShooting();
	GetWorld()->GetTimerManager().ClearTimer(shootTimerHandle);
}



void AMyCharacter::ShootProjectile() {
	Ray();
}

void AMyCharacter::Ray()
{
	FVector start = GetActorLocation();

	//FVector forward = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(characterMesh->GetRightVector(), 0.8);
	FVector forward = characterMesh->GetRightVector();
	forward.Z = 0;

	start = FVector(start.X + (forward.X * RayOffset), start.Y + (forward.Y * RayOffset), start.Z + (forward.Z * RayOffset));
	//maybe need to change end pos for randomness
	FVector end = start + forward * RayLength;

	FHitResult hit;


	if (GetWorld()) {
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Pawn, QueryParams, FCollisionResponseParams());
		NotifyFiredShot(GetActorForwardVector());
		//DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.15f, 0.f, 10.f);
		if (actorHit && hit.GetActor()) {
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, hit.GetActor()->GetFName().ToString());
			if (AEnemyCharacter* enemy = Cast<AEnemyCharacter>(hit.GetActor())) {
				enemy->TakeDamage(hit.Distance, Damage - FMath::RandRange(0, 3), GetActorLocation(), 2);
			}
			if (AObstacle* obstacle = Cast<AObstacle>(hit.GetActor())) {
				obstacle->DamageObstacle(Damage);
			}
		}

	}
}

#pragma endregion Shooting


void AMyCharacter::TakeDamage(float damageToTake) {
	currentHealth -= damageToTake;
	NotifyHealthBarWidget();
	NotifyTakeDamage();
	if (currentHealth <= 0) {
		DespawnPlayer();
	}
}

void AMyCharacter::DespawnPlayer()
{
	if (IsPlayerDead) {
		return;
	}
	IsPlayerDead = true;
	if (IsShooting) {
		ShootReleased();
	}
	NotifyPlayerDied();
	currentRespawnTime = TotalRespawnTime;
	respawnTimerHandle = UKismetSystemLibrary::K2_SetTimer(this, TEXT("UpdateRespawnTimer"), 1, true, 0, 0);
	trainPtr->StartRespawnTimer(PlayerIndex, currentRespawnTime);
	GetWorld()->GetSubsystem<UPlayerManagerWSS>()->CheckGameOver();
}

void AMyCharacter::UpdateRespawnTimer()
{
	currentRespawnTime--;
	trainPtr->UpdateTimer(PlayerIndex, currentRespawnTime);
	if (currentRespawnTime <= 0) {
		UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(this, respawnTimerHandle);
		ResetPlayer();
	}
}

void AMyCharacter::ResetPlayer() {
	SetActorLocation(trainPtr->GetRespawnPos(PlayerIndex));
	currentHealth = MaxHealth;
	IsPlayerDead = false;
	NotifyHealthBarWidget();
	NotifyPlayerRespawn();
	trainPtr->StopRespawnTimer(PlayerIndex, 0);
}

FVector AMyCharacter::GetRespawnLocation()
{
	return trainPtr->GetRespawnPos(PlayerIndex);
}

#pragma region Cosmetic

FColor AMyCharacter::GetPlayerColor()
{
	return FColor(PlayerColor.X * 255, PlayerColor.Y * 255, PlayerColor.Z * 255, 255);
}

bool AMyCharacter::HasFuel()
{
	if (Carrying && carriedObject != NULL) {
		return true;
	}
	return false;
}

void AMyCharacter::RegenerateHealth()
{
	currentHealth += RegenRate;
	if (currentHealth > MaxHealth) {
		currentHealth = MaxHealth;
	}
}

FVector AMyCharacter::SetPlayerColorVector(int index)
{
	switch (index) {
	case 0:
		return FVector(0, 0.4, 1);
	case 1:
		return FVector(0.4, 1, 0);
	case 2:
		return FVector(1, 0.5, 0);
	case 3:
		return FVector(0, 0.5, 0.5);
	default:
		return FVector(255, 255, 255);
	}
}

#pragma endregion Cosmetic

