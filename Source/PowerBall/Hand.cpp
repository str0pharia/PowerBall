// Fill out your copyright notice in the Description page of Project Settings.


#include "Hand.h"
#include "Components/SplineComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AHand::AHand()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HandMesh = FindComponentByClass<USkeletalMeshComponent>();

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	if ( SplineComponent != nullptr) 
	{
		SplineComponent->bDrawDebug = true;

	}

}


USplineComponent* AHand::GetSplineComponent()
{
	return SplineComponent;
}

void AHand::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

