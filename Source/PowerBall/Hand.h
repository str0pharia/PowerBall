// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hand.generated.h"

class USplineComponent;
class USkeletalMeshComponent;


UENUM(BlueprintType)
enum EGiantHandState 
{
	Default			UMETA(DisplayName = "Default"),
	Fist			UMETA(DisplayName = "Fist"),
	Grab_Crush 		UMETA(DisplayName = "Catch/Grab"),
	Swipe_Slap		UMETA(DisplayName = "Swipe"),
	Poke			UMETA(DisplayName = "Poke"),
	Stop			UMETA(DisplayName = "Stop"),
	Crush_Flatten	UMETA(DisplayName = "Stop"),
	Finger_Gun		UMETA(DisplayName = "FingerGun")

};

UCLASS()
class POWERBALL_API AHand : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHand();

	USplineComponent* GetSplineComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hand State/Pose")
	TEnumAsByte<EGiantHandState> State = EGiantHandState::Default;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	USkeletalMeshComponent* HandMesh = nullptr;

	USplineComponent* SplineComponent = nullptr;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
