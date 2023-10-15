// Fill out your copyright notice in the Description page of Project Settings.



#include "Train.h"
#include "GameManagerWSS.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyCharacter.h"


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
			if (BoxCollisionComponent && BoxCollisionComponent->ComponentHasTag("Plow"))
			{
				// You've found the Box Collision component with the specified tag.
				// You can perform operations on it here.
				plow = BoxCollisionComponent;
				plow->OnComponentBeginOverlap.AddDynamic(this, &ATrain::OnPlowBeginOverlap);
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
	if (AEnemyCharacter* enemy = Cast<AEnemyCharacter>(OtherActor)) {

		//GEngine->AddOnScreenDebugMessage(1, 3, FColor::Red, TEXT("TRAIN HIT ENEMY BOI"));
		enemy->TakeDamage(0, trainSpeed * .1, GetActorLocation(), trainSpeed * .01);
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