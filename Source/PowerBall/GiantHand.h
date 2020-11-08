// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
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

		virtual void TraceTarget();

		UFUNCTION(Server, Reliable) 
		virtual void ServerTraceTarget();
		
		virtual void Tick(float DeltaTime) override;

		virtual void BeginPlay() override;
	
		UPROPERTY(EditDefaultsOnly, Category = "Summon Blueprint")
		TSubclassOf<AHand> HandTemplate;

		UFUNCTION(BlueprintCallable,Category = "Runtime")
		FVector GetTraceHit();

	private:

		AHand* HandObjInstance = nullptr;		

		FTimerHandle PrimaryActionTimer;

		FTimerHandle AutoDestructTimer;

		float LastTraceHitTimeStamp = 0.0f;

		FVector LastTraceHit;

		USceneComponent* LockedTarget = nullptr;


	
};
