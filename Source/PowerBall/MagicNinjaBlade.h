// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "MagicNinjaBlade.generated.h"

/**
 * 
 */
UCLASS()
class POWERBALL_API AMagicNinjaBlade : public AWeapon
{
	GENERATED_BODY()


	protected: 

		FTimerHandle HitTimer;

		int HitCounter = 0;

	public:	

		AMagicNinjaBlade();

		void SpawnEffects(FVector TraceEnd) override;

		void Fire() override; 

		void ServerFire() override;

		void Hit();

	
};
