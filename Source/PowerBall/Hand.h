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
	Swipe_Slap		UMETA(DisplayName = "Swipe"),
	Poke			UMETA(DisplayName = "Poke"),
	Stop			UMETA(DisplayName = "Stop"),
	Crush_Flatten	UMETA(DisplayName = "Stop"),
	Finger_Gun		UMETA(DisplayName = "FingerGun")

};

UCLASS()
class POWERBALL_API AHand : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHand();

	USkeletalMeshComponent* GetMesh();

	void ExecuteAction(EGiantHandState State);
	void BeginAction();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hand State/Pose")
	TEnumAsByte<EGiantHandState> _State = EGiantHandState::Default;

	UProjectileMovementComponent* GetProjectileMovement();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	USkeletalMeshComponent* HandMesh = nullptr;

	UProjectileMovementComponent* ProjectileMovement = nullptr;

	
	FTimerHandle ActionTimer;

public:	
	// Called every frame

};