/*
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

*/


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hand.generated.h"
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


class UProjectileMovementComponent;
class USphereComponent;


UCLASS()
class AHand : public AActor
{
	GENERATED_BODY()

protected:

	/** Sphere collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Projectile")
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

public:

	AHand();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};