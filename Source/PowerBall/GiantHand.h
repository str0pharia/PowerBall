// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Hand.h"
#include "GiantHand.generated.h"

class AHand;
class USceneComponent;

/**
 * 
 */
UCLASS()
class POWERBALL_API AGiantHand : public AWeapon
{
	GENERATED_BODY()

	public:

		AGiantHand();

		void Fire() override;

		void ServerFire() override;

		void StopFire() override;

		void ServerStopFire() override;

		void SpawnEffects(FVector TraceEnd) override;

		void SetAttackState(EGiantHandState State);

		EGiantHandState	GetAttackState();

		virtual void Tick(float DeltaTime) override;

		virtual void BeginPlay() override;

		UPROPERTY(EditDefaultsOnly,Category = "Setup")
		TSubclassOf<AHand> HandTemplate;

		void OnRep_ProjectileInstance() override;
		
	private:

	
		FTimerHandle PrimaryActionTimer;

		EGiantHandState AttackState = EGiantHandState::Default;



	
};
