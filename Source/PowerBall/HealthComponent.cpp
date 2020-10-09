// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	DefaultHealth = 100; 
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* MyOwner = GetOwner();

	if (MyOwner) 
	{


		MyOwner->OnTakeAnyDamage.AddDynamic(this,&UHealthComponent::HandleTakeAnyDamage);
	}

	// ...
	Health = DefaultHealth;
}

void UHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{

	if ( Damage <= 0.0f )
	{

		return;
	}

	Health = FMath::Clamp(Health - Damage,0.0f,DefaultHealth);

	OnHealthChanged.Broadcast(this,Health, Damage, DamageType, InstigatedBy, DamageCauser);

}

