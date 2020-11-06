// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Components/SplineComponent.h"
#include "GiantHand.generated.h"

class USplineComponent;
struct FSplinePoint;
class AHand;

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

		void SpawnEffects(FVector TraceEnd) override;

		virtual void Tick(float DeltaTime) override;

		virtual void BeginPlay() override;

		void AddSplinePoint();

		void SetOrigin(FSplinePoint SplinePoint);
	
		UPROPERTY(EditDefaultsOnly, Category = "Template")
		TSubclassOf<AHand> HandTemplate;


	private:

		bool Recording = false;

		float MaxTicks = 5.0f;

		AHand* HandObjInstance = nullptr;		


	//	USplineMeshComponent* Spline = nullptr;

		FTimerHandle RecordPathTimer;


		TArray<FSplinePoint> SplinePoints;


	
};
