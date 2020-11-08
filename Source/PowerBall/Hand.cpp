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
	SetReplicates(true);

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
