// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hand.generated.h"


class UCapsuleComponent;
class USplineComponent;
class USkeletalMesh;
class UProjectileMovementComponent;

UENUM(BlueprintType)
enum EGiantHandState 
{
	Default			UMETA(DisplayName = "Default"),
	Fist			UMETA(DisplayName = "Fist"),
	Grab_Crush 		UMETA(DisplayName = "Catch/Grab"),
	Poke			UMETA(DisplayName = "Poke"),
	Stop			UMETA(DisplayName = "Stop"),
	Crush_Flatten	UMETA(DisplayName = "Crush"),
	Finger_Gun		UMETA(DisplayName = "FingerGun")

};

UCLASS()
class POWERBALL_API AHand : public APawn
{
	GENERATED_BODY()
	
public:	

	AHand();

	USkeletalMeshComponent* GetMesh();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hand State/Pose")
	TEnumAsByte<EGiantHandState> _State = EGiantHandState::Default;

	UProjectileMovementComponent* GetProjectileMovement();

	virtual void BeginPlay() override;
	

protected:

	USkeletalMeshComponent* HandMesh = nullptr;

	UProjectileMovementComponent* ProjectileMovement = nullptr;

	FTimerHandle AutoDestructTimer;

	void DestroyHand();


};
