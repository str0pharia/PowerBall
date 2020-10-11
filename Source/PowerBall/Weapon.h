// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class USkeletalMeshComponent;
class UParticleSystem;
class UDamageType;

UCLASS()
class POWERBALL_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	UPROPERTY(EditAnywhere, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh = nullptr;

	void Fire();

	UFUNCTION(Server, Reliable, WithValidation) 
	void ServerFire();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* TriggerEffect;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* FleshImpactEffect;


	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* FleshCriticalImpactEffect;

	void SpawnEffects(FVector TraceEnd);

	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Weapon")
	FName EffectOriginSocketName;



};
