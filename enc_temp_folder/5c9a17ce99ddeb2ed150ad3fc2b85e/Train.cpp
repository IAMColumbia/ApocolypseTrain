// Fill out your copyright notice in the Description page of Project Settings.



#include "Train.h"
#include "GameManagerWSS.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyCharacter.h"
#include "PlayerManagerWSS.h"
#include "Obstacle.h"

// Sets default values
ATrain::ATrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CanMove = true;
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

			break;
		case ETrainState::accelerating:
			if (currentTrainSpeed < MaxTrainSpeed) {
				currentTrainSpeed += AccelerationRate;
			}
			BurnFuel();
			break;
		case ETrainState::decelerating:
			if (isReversing) {
				if (currentTrainSpeed < 0) {
					currentTrainSpeed += DecelerationRate;
				}
				else {
					isReversing = false;
					currentTrainSpeed = 0;
				}
			}
			else {
				if (currentTrainSpeed > 0) {
					currentTrainSpeed -= DecelerationRate;
				}
				else {
					currentTrainSpeed = 0;
					SetTrainState(ETrainState::stopped);
				}
			}
			break;
		default:
			break;
	}
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
		currentLocation = GetActorLocation();
		currentLocation += FVector(0, 1, 0) * currentTrainSpeed * DeltaTime;
		SetActorLocation(currentLocation, true);
	}
	if (currentLocation.Y >= targetYPos) {
		GetWorld()->GetSubsystem<UGameManagerWSS>()->SpawnNewChunk();
	}

	if (!HasFuel() || leverState == ELeverState::stop && currentState == ETrainState::accelerating) {
		SetTrainState(ETrainState::decelerating);
	}
	if (leverState == ELeverState::move && HasFuel() && currentState != ETrainState::accelerating) {
		SetTrainState(ETrainState::accelerating);
	}
	//DrawDebugBox(GetWorld(), fuelDeposit->GetComponentLocation(), fuelDeposit->GetScaledBoxExtent(), FColor::Orange, false, -1.0f, 0U, 10.0f);
	//DrawDebugBox(GetWorld(), startBox->GetComponentLocation(), startBox->GetScaledBoxExtent(), FColor::Green, false, -1.0f, 0U, 10.0f);
	//DrawDebugBox(GetWorld(), stopBox->GetComponentLocation(), stopBox->GetScaledBoxExtent(), FColor::Red, false, -1.0f, 0U, 10.0f);
	MovementUpdate();
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
		/*if (enemy->EnemyState == EEnemyState::Dead) {
			enemy->Destroy();
		}*/
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
