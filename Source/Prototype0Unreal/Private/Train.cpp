// Fill out your copyright notice in the Description page of Project Settings.



#include "Train.h"
#include "GameManagerWSS.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyCharacter.h"
#include "PlayerManagerWSS.h"
#include "Obstacle.h"
#include "MyCharacter.h"
#include <Internationalization/Text.h>

// Sets default values
ATrain::ATrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CanMove = false;
	Fuel = MaxFuel;
	currentState = ETrainState::starting;
	graceTimeLeft = GraceTime;
}

void ATrain::StartTrain()
{
	if (HasFuel() && currentState != ETrainState::accelerating) {
		SetTrainState(ETrainState::accelerating);
	}
}



void ATrain::StopTrain()
{
	if (currentState == ETrainState::accelerating) {
		SetTrainState(ETrainState::decelerating);
	}
	
}

void ATrain::MovementUpdate()
{
	switch (currentState) {
		case ETrainState::stopped:
			SetTrainState(ETrainState::accelerating);
			break;
		case ETrainState::starting:
			if (currentTrainSpeed < tutorialMaxTrainSpeed) {
				currentTrainSpeed += AccelerationRate;
			}
			if (GetActorLocation().Y > TutorialEnd) {
				SetTrainState(ETrainState::accelerating);
			}
			break;
		case ETrainState::accelerating:
			if (currentTrainSpeed < MaxTrainSpeed) {
				currentTrainSpeed += AccelerationRate;
			}
			break;
		case ETrainState::decelerating:
			if (isReversing) {
				if (currentTrainSpeed < 0) {
					currentTrainSpeed += DecelerationRate;
				}
				else {
					isReversing = false;
				}
			}
			else {
				if (currentTrainSpeed > 0) {
					currentTrainSpeed -= DecelerationRate;
				}
				else {
					/*currentTrainSpeed = 0;
					SetTrainState(ETrainState::stopped);*/
					currentTrainSpeed = 0;
					SetTrainState(ETrainState::accelerating);
				}
			}
			break;
		default:
			break;
	}
	BurnFuel();
}

bool ATrain::HasFuel()
{
	if (Fuel > 0) {
		return true;
	}
	return false;
}

// Called when the game starts or when spawned
void ATrain::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetSubsystem<UGameManagerWSS>()->train = this;
	targetYPos = GetWorld()->GetSubsystem<UGameManagerWSS>()->GetNextTargetLocation();
	TotalMeters = 0;
	TSet<UActorComponent*> components = GetComponents();
	for (UActorComponent* Component : components)
	{
		if (Component->IsA(UBoxComponent::StaticClass()))
		{
			UBoxComponent* BoxCollisionComponent = Cast<UBoxComponent>(Component);
			if (BoxCollisionComponent && BoxCollisionComponent->ComponentHasTag("FuelDeposit"))
			{
				fuelDeposit = BoxCollisionComponent;
			}
			if (BoxCollisionComponent && BoxCollisionComponent->ComponentHasTag("Plow"))
			{
				plow = BoxCollisionComponent;
				plow->OnComponentBeginOverlap.AddDynamic(this, &ATrain::OnPlowBeginOverlap);
			}
			if (BoxCollisionComponent && BoxCollisionComponent->ComponentHasTag("startBox"))
			{
				leverBox = BoxCollisionComponent;
			}
		}
		if (Component->IsA(UStaticMeshComponent::StaticClass())) {
			UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(Component);
			if (mesh && mesh->ComponentHasTag("StartLever")) {
				startLeverMesh = mesh;
			}
		}
	}
	GetWorld()->GetTimerManager().SetTimer(startTimerHandle, this, &ATrain::TrainCanMove, 5.0, false);
}

void ATrain::PlayerJoined()
{
	burnRate += burnRateDifficultyScaling;
}

bool ATrain::FuelIsCritical()
{
	if (FuelState == EFuelState::critical) {
		return true;
	}
	return false;
}

void ATrain::TrainCanMove()
{
	CanMove = true;
	NotifyTrainStartGame();
}

void ATrain::IncrementTotalMeters()
{
	if (CanMove) {
		TotalMeters++;
		NotifyIncrementMeters(TotalMeters);
	}
}

// Called every frame
void ATrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CanMove) {
		MovementUpdate();
		currentLocation = GetActorLocation();
		currentLocation += FVector(0, 1, 0) * currentTrainSpeed * DeltaTime;
		SetActorLocation(currentLocation, true);
	}
	if (currentLocation.Y >= targetYPos) {
		GetWorld()->GetSubsystem<UGameManagerWSS>()->SpawnNewChunk();
	}

	/*if (!HasFuel() || leverState == ELeverState::stop && currentState == ETrainState::accelerating) {
		SetTrainState(ETrainState::decelerating);
	}
	if (leverState == ELeverState::move && HasFuel() && currentState != ETrainState::accelerating) {
		SetTrainState(ETrainState::accelerating);
	}*/
	//DrawDebugBox(GetWorld(), fuelDeposit->GetComponentLocation(), fuelDeposit->GetScaledBoxExtent(), FColor::Orange, false, -1.0f, 0U, 10.0f);
	//DrawDebugBox(GetWorld(), startBox->GetComponentLocation(), startBox->GetScaledBoxExtent(), FColor::Green, false, -1.0f, 0U, 10.0f);
	//DrawDebugBox(GetWorld(), stopBox->GetComponentLocation(), stopBox->GetScaledBoxExtent(), FColor::Red, false, -1.0f, 0U, 10.0f);
	UpdateFuelState();
}

bool ATrain::IsOverlappingFuelBox(FVector actorPos) {
	if (UKismetMathLibrary::IsPointInBox(actorPos, fuelDeposit->GetComponentLocation(), fuelDeposit->GetScaledBoxExtent())) {
		return true;
	}
	return false;
}

bool ATrain::IsOverlappingLeverBox(FVector actorPos) {
	if (UKismetMathLibrary::IsPointInBox(actorPos, leverBox->GetComponentLocation(), leverBox->GetScaledBoxExtent())) {
		return true;
	}
	return false;
	
}

void ATrain::ToggleTrainState()
{
	switch (leverState) {
	case ELeverState::stop:
		leverState = ELeverState::move;
		break;
	case ELeverState::move:
		leverState = ELeverState::stop;
		break;
	}
	LeverStateChanged();
}

bool ATrain::AddFuel() {
	if (Fuel + 1 <= MaxFuel) {
		Fuel++;
		NotifyFuelAdded();
		//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Red, FString::Printf(TEXT("fuel = %f"), Fuel));
		return true;
	}
	else {
		//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Red, FString::Printf(TEXT("fuel full. Max fuel = %f"), MaxFuel));
		return false;
	}
}

void ATrain::BurnFuel() {
	Fuel -= burnRate;
	if (Fuel < 0) {
		Fuel = 0;
		SetTrainState(ETrainState::decelerating);
	}
}

void ATrain::UpdateFuelState()
{
	if (Fuel > MaxFuel / 5) {
		FuelState = EFuelState::normal;
		NotifyFuelNormal();
	}
	else if (Fuel <= MaxFuel / 5) {
		FuelState = EFuelState::critical;
		NotifyFuelCritical();
		//notify danger
	}
	if (!HasFuel()) {
		//notify failed
		if (!countingDownGameOver && !GetWorld()->GetSubsystem<UGameManagerWSS>()->gameEnded) {
			countingDownGameOver = true;
			NotifyTrainStop();
			graceTimeLeft = GraceTime;
			GetWorld()->GetTimerManager().SetTimer(countdown, this, &ATrain::DecrementGameOverCounter, 1, true);
		}
	}
	if (countingDownGameOver) {
		if (HasFuel()) {
			NotifyTrainStart();
			GetWorld()->GetTimerManager().ClearTimer(countdown);
			countingDownGameOver = false;
		}
	}
}

void ATrain::DecrementGameOverCounter()
{
	NotifyGameOverCounter();
	if (graceTimeLeft <= 0 && !HasFuel()) {
		GetWorld()->GetTimerManager().ClearTimer(countdown);
		GetWorld()->GetSubsystem<UGameManagerWSS>()->GameOver(0);
	}
	graceTimeLeft--;
}

FVector ATrain::GetRandomRespawnPos() {
	int posChoice = UKismetMathLibrary::RandomInteger(4);
	switch (posChoice) {
		case 0:
			return GetActorLocation() + FVector(200, -100, 0);
		case 1:
			return GetActorLocation() + FVector(-200, -100, 0);
		case 2:
			return GetActorLocation() + FVector(200, -400, 0);
		case 3:
			return GetActorLocation() + FVector(-200, -400, 0);
		default:
			return GetActorLocation() + FVector(-200, -100, 0);
	}
}

float ATrain::GetBackBound()
{
	return GetActorLocation().Y - BackBound;
}

float ATrain::GetFrontBound()
{
	return BackBound + GetActorLocation().Y;
}

void ATrain::OnPlowBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (currentTrainSpeed <= minnimumDamagingSpeed) {
		return;
	}
	if (AEnemyCharacter* enemy = Cast<AEnemyCharacter>(OtherActor)) {
		
		//GEngine->AddOnScreenDebugMessage(1, 3, FColor::Red, TEXT("TRAIN HIT ENEMY BOI"));
		enemy->TakeDamage(0, currentTrainSpeed * damageMultiplier, GetActorLocation(), currentTrainSpeed * launchMultiplier);
		NotifyTrainHitEnemy();
		/*if (enemy->EnemyState == EEnemyState::Dead) {
			enemy->Destroy();
		}*/
	}
	if (AMyCharacter* player = Cast<AMyCharacter>(OtherActor)) {
		player->TakeDamage(playerDamage);
		player->LaunchCharacter(GetActorRightVector() * playerLaunchForce, true, true);
	}
	if (OtherActor->Tags.Contains("Obstacle")) {
		currentTrainSpeed *= -1;
		isReversing = true;
		//SetTrainState(ETrainState::decelerating);
		ToggleTrainState();
		NotifyTrainHitObstacle();
		if (AObstacle* obstacle = Cast<AObstacle>(OtherActor)) {

			obstacle->DamageObstacle(currentTrainSpeed);
		}
		Fuel -= CollisionFuelLoss;
	}
}

FVector ATrain::GetRespawnPos(int PlayerIndex) {
	switch (PlayerIndex) {
		case 0:
			return player0RespawnPos + GetActorLocation();
		case 1:
			return player1RespawnPos + GetActorLocation();
		case 2:
			return player2RespawnPos + GetActorLocation();
		case 3:
			return player3RespawnPos + GetActorLocation();
		default:
			return player1RespawnPos + GetActorLocation();
	}
}

void ATrain::SetTrainState(ETrainState stateToSet)
{
	switch (stateToSet) {
		case ETrainState::stopped:
			GetWorld()->GetSubsystem<UGameManagerWSS>()->OnTrainStopped();
			break;
		case ETrainState::accelerating:
			GetWorld()->GetSubsystem<UGameManagerWSS>()->OnTrainAccelerating();
			break;
		case ETrainState::decelerating:
			break;
	}
	currentState = stateToSet;
}
