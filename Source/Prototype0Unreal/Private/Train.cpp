// Fill out your copyright notice in the Description page of Project Settings.



#include "Train.h"
#include "GameManagerWSS.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ATrain::ATrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CanMove = true;
}

// Called when the game starts or when spawned
void ATrain::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetSubsystem<UGameManagerWSS>()->train = this;
	TotalMeters = 0;
	for (UActorComponent* Component : GetComponents())
	{
		if (Component->IsA(UBoxComponent::StaticClass()))
		{
			UBoxComponent* BoxCollisionComponent = Cast<UBoxComponent>(Component);
			if (BoxCollisionComponent && BoxCollisionComponent->ComponentHasTag("FuelDeposit"))
			{
				// You've found the Box Collision component with the specified tag.
				// You can perform operations on it here.
				fuelDeposit = BoxCollisionComponent;
			}
		}
	}
}

void ATrain::IncrementTotalMeters()
{
	if (CanMove) {
		TotalMeters++;
	}
}

// Called every frame
void ATrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CanMove) {
		currentLocation = GetActorLocation();
		currentLocation += FVector(0, 1, 0) * trainSpeed * DeltaTime;
		SetActorLocation(currentLocation, true);
	}
	if (currentLocation.Y >= targetYPos) {
		GetWorld()->GetSubsystem<UGameManagerWSS>()->TrainArrivedAtTarget();
	}
	DrawDebugBox(GetWorld(), fuelDeposit->GetComponentLocation(), fuelDeposit->GetComponentScale() * 50, FColor::Orange, false, -1.0f, 0U, 10.0f);
	BurnFuel();
}

bool ATrain::IsOverlappingFuelBox(FVector actorPos) {
	if (UKismetMathLibrary::IsPointInBox(actorPos, fuelDeposit->GetComponentLocation(), fuelDeposit->GetComponentScale() * 50)) {
		return true;
	}
	return false;
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
	trainSpeed = Fuel * SpeedFuelMultiplier;
	Fuel -= burnRate;
	if (Fuel < 0) {
		Fuel = 0;
	}
}

FVector ATrain::GetRandomRespawnPos() {
	int posChoice = UKismetMathLibrary::RandomInteger(4);
	switch (posChoice) {
		case 0:
			return GetActorLocation() + FVector(200, 0, 0);
		case 1:
			return GetActorLocation() + FVector(-200, 0, 0);
		case 2:
			return GetActorLocation() + FVector(200, -300, 0);
		case 3:
			return GetActorLocation() + FVector(-200, -300, 0);
		default:
			return GetActorLocation() + FVector(-200, 0, 0);
	}
}

FVector ATrain::GetRespawnPos(int PlayerIndex) {
	switch (PlayerIndex) {
		case 0:
			return player1RespawnPos;
		case 1:
			return player2RespawnPos;
		case 2:
			return player3RespawnPos;
		case 3:
			return player4RespawnPos;
		default:
			return player1RespawnPos;
	}
}