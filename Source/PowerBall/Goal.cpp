// Fill out your copyright notice in the Description page of Project Settings.


#include "Goal.h"
#include "Components/SphereComponent.h"

// Sets default values
AGoal::AGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BaseCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PickUpcollision"));

	BaseCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BaseCollision->SetGenerateOverlapEvents(true);

	BaseCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	BaseCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel3,ECR_Overlap);
	BaseCollision->SetSphereRadius(145.0f);

	
	BaseCollision->OnComponentBeginOverlap.AddDynamic(this,&AGoal::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AGoal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGoal::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult )
{
	
	if ( OtherActor->ActorHasTag(FName("Ball")) )
	{
		UE_LOG(LogTemp,Warning,TEXT("Overlap ball!"));
	}

}