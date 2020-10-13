// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class UHealthComponent;
class AWeapon;
class UInputComponent;
class USkeletalMeshComponent;
class APowerBallGameState;

UCLASS()
class POWERBALL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraObject;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	bool bDied = false;

	UFUNCTION()
	void OnHealthChanged(UHealthComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(Replicated,VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AWeapon* CurrentWeapon;

	void PrimaryActionStart();

	void PrimaryActionStop();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual FVector GetPawnViewLocation() const override; 

	FVector GetBallSocketLocation();

	bool PossessesBall();

	UHealthComponent* HealthComponent; 

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<AWeapon> DefaultWeapon; 


	bool IsAlive();


  protected:

	virtual void BeginPlay() override;

	void MoveForward(float axis);

	void MoveRight(float axis);	


};
