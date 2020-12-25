// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UHealthComponent;
class AWeapon;
class AProjectileWeapon;
class UInputComponent;
class USkeletalMeshComponent;
class APowerBallGameState;
class USphereCollision;
class AHand;

UCLASS()
class POWERBALL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	APlayerCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraObject;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	bool bDied = false;

	UPROPERTY(Replicated,VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AWeapon* CurrentWeapon;

	UPROPERTY(ReplicatedUsing=OnRep_PrimaryAction,VisibleAnywhere, BluePrintReadWrite, Category = "States")
	bool bPrimaryAction = false;

	UPROPERTY(ReplicatedUsing=OnRep_SecondaryAction,VisibleAnywhere, BluePrintReadWrite, Category = "States")
	bool bSecondaryAction = false;

	UPROPERTY(ReplicatedUsing=OnRep_PushBallAction,VisibleAnywhere, BluePrintReadWrite, Category = "States")
	bool bPushBallAction = false;

	UPROPERTY(ReplicatedUsing=OnRep_PullBallAction,VisibleAnywhere, BluePrintReadWrite, Category = "States")
	bool bPullBallAction = false;

	//UPROPERTY(ReplicatedUsing=OnRep_TelekineticAction,VisibleAnywhere, BluePrintReadWrite, Category = "States")
	//bool bPrimaryDefensiveAction = false;

	//UPROPERTY(ReplicatedUsing=OnRep_TelekineticAction,VisibleAnywhere, BluePrintReadWrite, Category = "States")
	//bool bSecondaryDefensiveAction = false;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void PrimaryActionPressed();
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void PrimaryActionReleased();

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void SecondaryActionPressed();
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void SecondaryActionReleased();


	UFUNCTION(BlueprintCallable, Category = "Actions")
	void BeginPushBallAction();
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void EndPushBallAction();
	

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void BeginPullBallAction();
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void EndPullBallAction();
	

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AbortAction();

	virtual FVector GetPawnViewLocation() const override; 

	UFUNCTION()
	void OnRep_PrimaryAction();

	UFUNCTION()
	void OnRep_SecondaryAction();

	UFUNCTION()
	void OnRep_PushBallAction();

	UFUNCTION()
	void OnRep_PullBallAction();
	UHealthComponent* HealthComponent; 

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<AWeapon> DefaultWeapon; 

	UFUNCTION()
	void OnHealthChanged(UHealthComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	bool IsAlive();	

	USkeletalMeshComponent* GetPlayerMesh();
	
	UFUNCTION(Server, Reliable, WithValidation) 
	virtual void ServerBeginPushBallAction();

	UFUNCTION(Server, Reliable, WithValidation) 
	virtual void ServerBeginPullBallAction();

	void PushBall();
	void PullBall();
	UFUNCTION(Server, Reliable, WithValidation) 
	void ServerPushBall();

	UFUNCTION(Server, Reliable, WithValidation) 
	void ServerPullBall();

  protected:

	virtual void BeginPlay() override;

	void MoveForward(float axis);

	void MoveRight(float axis);	

	FTimerHandle PrimaryActionTimer;
	FTimerHandle PushBallActionTimer;
	FTimerHandle PullBallActionTimer;
	
	USkeletalMeshComponent* PlayerMesh;
	
	float PushBallActionTimestamp;
	float PullBallActionTimestamp;


};
