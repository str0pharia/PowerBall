// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "HealthComponent.h"
#include "Weapon.h"
#include "PowerBallGameState.h"
#include "BasketBall.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(RootComponent);

	CameraObject = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	CameraObject->SetupAttachment(SpringArm);

	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

	PlayerMesh = FindComponentByClass<USkeletalMeshComponent>();

	SetReplicates(true);
}

USkeletalMeshComponent* APlayerCharacter::GetPlayerMesh() {
	return PlayerMesh;
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{


	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	DOREPLIFETIME(APlayerCharacter, CurrentWeapon);



}



// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnHealthChanged.AddDynamic(this, &APlayerCharacter::OnHealthChanged);



		FActorSpawnParameters SpawnParams; 
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultWeapon,FVector(0),FRotator(0),SpawnParams);
		if ( CurrentWeapon != nullptr ) 
		{	
			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(FindComponentByClass<USkeletalMeshComponent>(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,"WeaponSocket");
		}

}





// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector APlayerCharacter::GetBallSocketLocation()
{
	auto mesh = FindComponentByClass<USkeletalMeshComponent>();
	return mesh->GetSocketLocation(FName("BallSocket"));

	

}


void APlayerCharacter::OnHealthChanged(UHealthComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{

	if ( Health <= 0.0f && !bDied) 
	{

		bDied = true;

		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}



}


void APlayerCharacter::PrimaryActionStart()
{
		if ( CurrentWeapon != nullptr) {
			CurrentWeapon->Fire();
		}
	
}

void APlayerCharacter::PrimaryActionStop()
{


}

void APlayerCharacter::MoveForward(float axis) 
{


	AddMovementInput(GetActorForwardVector() * axis);


}


void APlayerCharacter::MoveRight(float axis) 
{


	AddMovementInput(GetActorRightVector() * axis);
	
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAxis("MoveForward",this,&APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp",this,&APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn",this,&APlayerCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("ActionButton_A",EInputEvent::IE_Pressed,this,&APlayerCharacter::PrimaryActionStart);
	PlayerInputComponent->BindAction("ActionButton_A",EInputEvent::IE_Released,this,&APlayerCharacter::PrimaryActionStop);
}

FVector APlayerCharacter::GetPawnViewLocation() const
{

	if ( CameraObject ) 
	{
		return CameraObject->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();

}

/*
bool APlayerCharacter::PossessesBall() 
{
	ABasketBall* B = (GetWorld()->GetGameState<APowerBallGameState>())->GetBasketBall();
	return (B->Possessor == this);
}
*/
bool APlayerCharacter::IsAlive() 
{

	return (HealthComponent->Health > 0);
}




