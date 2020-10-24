// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class POWERBALL_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()

	virtual void Fire() override;

	virtual void SpawnEffects(FVector TraceEnd) override;
	
};
