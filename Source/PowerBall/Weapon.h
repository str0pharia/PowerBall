// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class USkeletalMeshComponent;
class UParticleSystem;
class UDamageType;
class UAnimMontage;

USTRUCT()
struct FHitScanTrace
{

	GENERATED_BODY();

	public:

		UPROPERTY()
		FVector_NetQuantize TraceFrom;

		UPROPERTY()
		FVector_NetQuantize TraceTo;
	
};

UCLASS()
class POWERBALL_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	UPROPERTY(EditAnywhere, Category = "Weapon Mesh")
	USkeletalMeshComponent* WeaponMesh = nullptr;

	virtual void Fire();

	UFUNCTION(Server, Reliable, WithValidation) 
	virtual void ServerFire();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Weapon Data")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Weapon Effects")
	UParticleSystem* TriggerEffect;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Weapon Effects")
	UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Weapon Effects")
	UParticleSystem* FleshImpactEffect;


	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Weapon Effects")
	UParticleSystem* FleshCriticalImpactEffect;


	virtual void SpawnEffects(FVector TraceEnd);

	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Weapon Effects")
	UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Weapon Effects")
	FName EffectOriginSocketName;

	UPROPERTY(ReplicatedUsing=OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace; 

	UFUNCTION()
	void OnRep_HitScanTrace();

	float LastFireTime = 0;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Weapon Trigger")
	bool TriggerOnRelease = false;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Weapon Trigger")
	float HoldTriggerScalar = 0.0f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Weapon Animation")
	UAnimMontage* PrimaryActionMontage = nullptr;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Weapon Animation")
	UAnimMontage* SecondaryActionMontage = nullptr;

};
