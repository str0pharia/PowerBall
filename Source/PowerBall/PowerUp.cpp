// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"

// Sets default values
APowerUp::APowerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComp;

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	
	DecalComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APowerUp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APowerUp::NotifyActorBeginOverlap(AActor* Other)
{

	Super::NotifyActorBeginOverlap(Other);
}
