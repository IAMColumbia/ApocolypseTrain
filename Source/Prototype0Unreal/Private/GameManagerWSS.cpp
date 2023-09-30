// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManagerWSS.h"

void UGameManagerWSS::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("Subsystem Initialized"));
}


void UGameManagerWSS::PrintTrainLocation(FVector location) {
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Magenta, FString::Printf(TEXT("Train Location boii %f"), location.Y));
}

float UGameManagerWSS::GetNextTargetLocation() {
	return chunkSpawner->GetNextTargetLocation();
}

//FVector UGameManagerWSS::TrainArrived() {
//	return FVector::Zero();
//}
