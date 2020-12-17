// Fill out your copyright notice in the Description page of Project Settings.


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
	
	ProjectileMovement = FindComponentByClass<UProjectileMovementComponent>();

	bNetLoadOnClient = true;		
	SetReplicates(true);
}
		

USkeletalMeshComponent* AHand::GetMesh() 
{

	return HandMesh;
}

void AHand::BeginPlay() 
{

	Super::BeginPlay();

	if ( ProjectileMovement != nullptr ) 
	{
		GetWorldTimerManager().SetTimer(AutoDestructTimer,this,&AHand::DestroyHand,4.0f);

		ProjectileMovement->SetVelocityInLocalSpace(GetInstigator()->GetActorForwardVector() * 100.f);

		ProjectileMovement->Activate();
	}
}

UProjectileMovementComponent* AHand::GetProjectileMovement() 
{
	return ProjectileMovement;
}

void AHand::DestroyHand()
{
	GetWorldTimerManager().ClearTimer(AutoDestructTimer);
	UE_LOG(LogTemp,Warning,TEXT("Auto destruct timer expired, destroying hand ...."));
	GetRootComponent()->SetVisibility(false);
	this->Destroy();
}