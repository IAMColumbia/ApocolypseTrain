// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"
#include <Components/BoxComponent.h>

// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	TSet<UActorComponent*> components = GetComponents();
	for (UActorComponent* component : components) {
		if (component->IsA(UBoxComponent::StaticClass())) {
			UBoxComponent* BoxCollisionComponent = Cast<UBoxComponent>(component);
			if (BoxCollisionComponent && BoxCollisionComponent->ComponentHasTag("Trigger"))
			{
				trigger = BoxCollisionComponent;
			}
		}
	}
}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

