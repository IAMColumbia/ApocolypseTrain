// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Weapon.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	characterMesh = FindComponentByClass<USkeletalMeshComponent>();
	////Initialize the Camera Boom
	//CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	////Setup Camera Boom attatchment to the root component of the class
	//CameraBoom->SetupAttachment(RootComponent);

	////Set the boolean to use the PawnControlRotaion to true;
	//CameraBoom->bUsePawnControlRotation = true;

	////Initialize the FollowCamera
	//FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));

	////Set followcamera attachment to the camera boom
	//FollowCamera->SetupAttachment(CameraBoom);

	//PlayerWeapon->CreateDefaultSubobject<UWeapon>(TEXT("PlayerWeapon"));
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AMyCharacter::setXRot(GetInputAxisValue("MoveRight"));
	AMyCharacter::setYRot(GetInputAxisValue("MoveForward"));
	AMyCharacter::setRotation();
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//this dumbass bindaction shit is not working
	//PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AMyCharacter::ShootPressed);
	//PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AMyCharacter::ShootReleased);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
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
	Ray();
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("SHoot Pressed"));
}

void AMyCharacter::ShootReleased() {
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("SHoot Released"));
}

void AMyCharacter::MoveForward(float AxisValue) {
	if ((Controller != NULL) && AxisValue != 0.0f && !IsShooting) {
		//find out which direction is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, AxisValue);
	}
}

void AMyCharacter::MoveRight(float AxisValue) {
	if (Controller != NULL && AxisValue != 0.0f && !IsShooting) {

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
	start = FVector(start.X + (forward.X * RayLength), start.Y + (forward.Y * RayLength), start.Z + (forward.Z * RayLength));
	FVector end = start + (forward * 1000);

	FHitResult hit;

	if (GetWorld()) {
		bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Pawn, FCollisionQueryParams(), FCollisionResponseParams());
		DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 2.0f, 0.f, 10.f);
		if (actorHit && hit.GetActor()) {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, hit.GetActor()->GetFName().ToString());
		}
	}
}

