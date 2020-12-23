// Fill out your copyright notice in the Description page of Project Settings.

/*
#include "Hand.h"
#include "Components/SplineComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AHand::AHand()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HandMesh = FindComponentByClass<USkeletalMeshComponent>();

	//ProjectileMovement = FindComponentByClass<UProjectileMovementComponent>();¨

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));

	ProjectileMovement->InitialSpeed = 1000.0f;

	SetReplicates(true);
	SetReplicateMovement(true);

}

void AHand::ExecuteAction(EGiantHandState State)
{
	
	_State = State;



}							

USkeletalMeshComponent* AHand::GetMesh() 
{
	return HandMesh;

}
void AHand::BeginPlay()
{
	Super::BeginPlay();


}

void AHand::BeginAction() 
{
		//GetWorldTimerManager().SetTimer(ActionTimer,this,&AHand::Move,0.2f,true,0.5);
	
}

UProjectileMovementComponent* AHand::GetProjectileMovement() 
{

	return ProjectileMovement;
}

*/

// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Hand.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AHand::AHand() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AHand::OnHit);	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	SetReplicates(true);
	SetReplicateMovement(true);
}


void AHand::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{



}