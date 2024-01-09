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
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon.h"

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

	//FVector forward = characterMesh->GetRightVector();
	FVector forward = CarrySlot->GetComponentLocation() - GetActorLocation();
	forward.Z = 0;

	//start = FVector(start.X + (forward.X), start.Y + (forward.Y), start.Z + (forward.Z));
	//maybe need to change end pos for randomness
	FVector end = start + (forward* 1.2 );

	FHitResult hit;

	if (GetWorld()) {
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(carriedObject);
		QueryParams.AddIgnoredActor(CurrentWeapon);
		bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_WorldDynamic,QueryParams, FCollisionResponseParams());
		//DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 4.15f, 0.f, 10.f);
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
	
	baseHealth = MaxHealth;
	currentHealth = MaxHealth;
	NotifyHealthBarWidget();
	gameManager = GetWorld()->GetSubsystem<UGameManagerWSS>();
	trainPtr = gameManager->train;

	TSet<UActorComponent*> components = GetComponents();
	for (UActorComponent* component : components) {
		if (component->IsA(USceneComponent::StaticClass())) {
			USceneComponent* sceneComponent = Cast<USceneComponent>(component);
			if (sceneComponent && sceneComponent->ComponentHasTag("HolsterSlot")) {
				HolsterSlot = sceneComponent;
			}
			if (sceneComponent && sceneComponent->ComponentHasTag("CarrySlot")) {
				CarrySlot = sceneComponent;
			}
		}
	}
	canDash = true;
	//PlayerWeapon = Cast<AWeapon>(AddComponentByClass(AWeapon::StaticClass(), true, GetActorTransform(), false));
	//PlayerWeapon = CreateDefaultSubobject<AWeapon>(TEXT("DefaultWeapon"));
	CurrentWeapon = Cast<AWeapon>(GetWorld()->SpawnActor(DefaultWeapon));
	CurrentWeapon->OwnerCharacter = this;
	AttachWeapon();
	
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
	//AMyCharacter::setXRot(GetInputAxisValue("Horizontal"));
	//AMyCharacter::setYRot(GetInputAxisValue("Vertical"));
	//AMyCharacter::setRotation();
	if (trainPtr->IsOverlappingFuelBox(GetActorLocation()) && Carrying) {
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
	ApplyDash();
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
	PlayerInputComponent->BindAction("Dash", EInputEvent::IE_Pressed, this, &AMyCharacter::DashPressed);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis("AimHorizontal", this, &AMyCharacter::setXRot);
	PlayerInputComponent->BindAxis("AimVertical", this, &AMyCharacter::setYRot);
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
	CheckDropItem();
}

void AMyCharacter::InteractReleased()
{
	Interacted = false;
	justDropped = false;
}

bool AMyCharacter::isWeaponUpright()
{
	if (CurrentWeapon != NULL) {
		return CurrentWeapon->upright;
	}
	return false;
}

void AMyCharacter::PickupItem(AInteractableActor* itemToCarry)
{
	if (Carrying) {
		return;
	}
	Carrying = true;
	carriedObject = itemToCarry;
	
	HolsterWeapon();
	itemToCarry->OnPickedUp(CarrySlot);
	ShootReleased();
	//itemToCarry->SetActorLocation(CarryPosition->GetComponentLocation());
	//itemToCarry->AttachToComponent(CarryPosition, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void AMyCharacter::CheckDropItem()
{
	if (Carrying) {
		if (carriedObject != NULL) {
			if (IsFacingWall()) {
				carriedObject->SetActorLocation(GetActorLocation());
				carriedObject->DropObject(characterMesh->GetRightVector() * 0.2);
			}
			else {
				carriedObject->DropObject(characterMesh->GetRightVector() * -1);
			}
		}
		Carrying = false;
		AttachWeapon();
		CurrentWeapon->WeaponEquipped();
		CurrentWeapon->ShowLaser();
		justDropped = true;
	}
}


void AMyCharacter::AttachWeapon()
{
	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	CurrentWeapon->SetActorLocation(characterMesh->GetSocketTransform("WeaponSocket").GetLocation());
	//CurrentWeapon->AttachToActor(this, rules);
	CurrentWeapon->AttachToComponent(characterMesh, rules, "WeaponSocket");
	CurrentWeapon->Equipped = true;
}

void AMyCharacter::HolsterWeapon()
{
	CurrentWeapon->Equipped = false;
	CurrentWeapon->LowerWeapon();
	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	CurrentWeapon->SetActorLocation(HolsterSlot->GetComponentLocation());
	CurrentWeapon->SetActorRotation(HolsterSlot->GetComponentRotation());
	//CurrentWeapon->AttachToActor(this, rules);
	CurrentWeapon->AttachToComponent(HolsterSlot, rules);
	CurrentWeapon->HideLaser();
}

TSubclassOf<AWeapon> AMyCharacter::PickupWeapon(TSubclassOf<AWeapon> weaponToPickup)
{
	TSubclassOf<AWeapon> droppedWeapon = CurrentWeapon->GetClass();
	if (!CurrentWeapon->IsA(weaponToPickup)) {
		CurrentWeapon->Destroy();
		CurrentWeapon = Cast<AWeapon>(GetWorld()->SpawnActor(weaponToPickup));
		CurrentWeapon->OwnerCharacter = this;
		AttachWeapon();
	}
	return droppedWeapon;
}

TSubclassOf<AWeapon> AMyCharacter::GetCurrentWeapon()
{
	return CurrentWeapon->StaticClass();
}

void AMyCharacter::setXRot(float AxisValue) {
	aimRot.X = AxisValue * -1;
	setRotation();
}

void AMyCharacter::setYRot(float AxisValue) {
	aimRot.Y = AxisValue;
	setRotation();
}


void AMyCharacter::MoveRight(float AxisValue) {
	DashDirection.Y = (AxisValue * -1);
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
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("X: %d  Y: %d"), aimRot.X, aimRot.Y));
	if (aimRot.Length() >= deadZone) {
		FRotator rotation = UKismetMathLibrary::MakeRotationFromAxes(aimRot, FVector::Zero(), FVector::Zero());
		characterMesh->SetWorldRotation(rotation + FRotator(0, 200, 0));
	}
}

void AMyCharacter::MoveForward(float AxisValue) {
	DashDirection.X = (AxisValue * -1);
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
	if (CurrentWeapon == NULL) {
		return;
	}
	IsShooting = true;
	NotifyStartedShooting();
	CurrentWeapon->Attack();
}

void AMyCharacter::ShootReleased() {
	IsShooting = false;
	NotifyStoppedShooting();
	CurrentWeapon->EndAttack();
}

void AMyCharacter::DashPressed()
{
	if (canDash) {
		canDash = false;
		isDashing = true;
		DashSpeed = DashForce;
		//stop player from shooting while dashing
		if (IsShooting) {
			ShootReleased();
		}
		GetWorld()->GetTimerManager().SetTimer(dashCooldownTimerHandle, this, &AMyCharacter::ResetDash,DashCooldown, false);
		GetWorld()->GetTimerManager().SetTimer(dashTimerHandle, this, &AMyCharacter::FinishDash, KnockBackDuration, false);
		//GetCharacterMovement()->MaxWalkSpeed = DashSpeed;
		if (DashDirection.X == 0 && DashDirection.Y == 0) {
			NotifyOnDash(characterMesh->GetRightVector()*-1);
		}
		else {
			NotifyOnDash(DashDirection);
		}
	}
}

void AMyCharacter::ApplyDash()
{
	if (isDashing) {
		FHitResult hit;
		FVector start = GetActorLocation();
		FVector end = start + (DashDirection* 70);
		if (GetWorld()) {
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);
			bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Pawn, QueryParams, FCollisionResponseParams());
			//DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.15f, 0.f, 10.f);
			if (actorHit && hit.GetActor()) {

			}
			else {
				FVector loc = GetActorLocation() + (DashDirection * DashSpeed);
				SetActorLocation(loc);
			}
		}
		DashSpeed -= DashDecelleration;
		if (DashSpeed <= 0) {
			DashSpeed = 0;
			isDashing = false;
		}
	}
}

void AMyCharacter::FinishDash() {
	isDashing = false;
	OnDashFinish();
	GetWorld()->GetTimerManager().ClearTimer(dashTimerHandle);
}

void AMyCharacter::ResetDash() {
	canDash = true;
	GetWorld()->GetTimerManager().ClearTimer(dashCooldownTimerHandle);
}

#pragma endregion Shooting

#pragma region spawning

void AMyCharacter::TakeDamage(float damageToTake) {
	if (Invincible) {
		return;
	}
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
	CheckDropItem();
	justDropped = false;
	NotifyPlayerDied();
	TotalDeaths++;
	MaxHealth = baseHealth;
	DamageBuff = 0;
	SpeedBuff = 0;
	currentRespawnTime = TotalRespawnTime;
	respawnTimerHandle = UKismetSystemLibrary::K2_SetTimer(this, TEXT("UpdateRespawnTimer"), 1, true, 0, 0);
	trainPtr->StartRespawnTimer(PlayerIndex, currentRespawnTime);
	GetWorld()->GetSubsystem<UPlayerManagerWSS>()->CheckGameOver();
}

void AMyCharacter::SetInvincibility()
{
	Invincible = false;
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
	Invincible = true;
	currentHealth = MaxHealth;
	IsPlayerDead = false;
	NotifyHealthBarWidget();
	NotifyPlayerRespawn();
	trainPtr->StopRespawnTimer(PlayerIndex, 0);
	GetWorld()->GetTimerManager().SetTimer(respawnTimerHandle, this, &AMyCharacter::SetInvincibility, 3, false);
}

FVector AMyCharacter::GetRespawnLocation()
{
	return trainPtr->GetRespawnPos(PlayerIndex);
}

#pragma endregion spawning

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
void AMyCharacter::Heal() {
	currentHealth = MaxHealth;
}

FVector AMyCharacter::SetPlayerColorVector(int index)
{
	switch (index) {
	case 0:
		return FVector(0, 0.4, 1);
	case 1:
		return FVector(1, 0, 0);
	case 2:
		return FVector(0.4, 1, 0);
	case 3:
		return FVector(1, 1, 0);
	default:
		return FVector(255, 255, 255);
	}
}

#pragma endregion Cosmetic

