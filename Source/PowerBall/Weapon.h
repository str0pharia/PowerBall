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

	UPROPERTY(EditAnywhere, Category = "Power-Up Equipped Mesh")
	USkeletalMeshComponent* WeaponMesh = nullptr;

	virtual void BeginPlay();

	virtual void Fire();

	virtual void StopFire();
	
	UFUNCTION(Server, Reliable, WithValidation) 
	virtual void ServerFire();

	UFUNCTION(Server, Reliable) 
	virtual void ServerStopFire();


	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Power-Up Data")
	float Damage;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Power-Up Data")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Power-Up Effects")
	UParticleSystem* TriggerEffect;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Power-Up Effects")
	UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Power-Up Effects")
	UParticleSystem* FleshImpactEffect;


	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Power-Up Effects")
	UParticleSystem* FleshCriticalImpactEffect;


	virtual void SpawnEffects(FVector TraceEnd);

	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Power-Up Effects")
	UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Power-Up Effects")
	FName EffectOriginSocketName;

	UPROPERTY(ReplicatedUsing=OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace; 

	UPROPERTY(ReplicatedUsing=OnRep_ProjectileInstance)
	AActor* ProjectileInstance; 

	UFUNCTION()
	void OnRep_HitScanTrace();

	UFUNCTION()
	virtual void OnRep_ProjectileInstance();

	float StartFireTime = 0;

	float StopFireTime = 0;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Power-Up Trigger")
	int Hits = 1;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Power-Up Trigger")
	float HitIntervalSeconds = 0.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Power-Up Trigger")
	float InitialDelaySeconds = 0.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Power-Up Trigger")
	float CoolDownSeconds = 1.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Power-Up Trigger")
	float Duration = 1.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Power-Up Trigger")
	bool TriggerOnRelease = false;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Power-Up Trigger")
	float HoldTriggerScalar = 0.0f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Power-Up Parameters")
	float MaxRange = 100.0f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Power-Up Parameters")
	float MinRange = 10.0f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Projectile Power-Up Parameters")
	float ProjectileLaunchSpeed = 5.0f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Power-Up Parameters")
	float Radius = 10.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Power-Up Animation")
	UAnimMontage* PrimaryActionMontage = nullptr;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Power-Up Animation")
	UAnimMontage* SecondaryActionMontage = nullptr;

};
