// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "HealthComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Weapon.h"
#include "Hand.h"
#include "PowerBallGameState.h"
#include "BasketBall.h"


struct FTimerHandle;

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
		DOREPLIFETIME(APlayerCharacter, bPrimaryAction);
		DOREPLIFETIME(APlayerCharacter, bSecondaryAction);
		DOREPLIFETIME(APlayerCharacter, bPushBallAction);
		DOREPLIFETIME(APlayerCharacter, bPullBallAction);

	}

	void APlayerCharacter::ServerBeginPushBallAction_Implementation() 
	{



	}

	bool APlayerCharacter::ServerBeginPushBallAction_Validate() 
	{

		return true;

	}
	void APlayerCharacter::ServerBeginPullBallAction_Implementation() 
	{



	}

	bool APlayerCharacter::ServerBeginPullBallAction_Validate() 
	{

		return true;

	}
	void APlayerCharacter::BeginPushBallAction() 
	{
	
		// get start timestamp
		// Set timer
		// play charge animation 

		/* RPC */
		if ( GetLocalRole() < ROLE_Authority )
		{
			ServerBeginPushBallAction();
		}

		PushBallActionTimestamp = GetWorld()->TimeSeconds;
		GetWorldTimerManager().SetTimer(PushBallActionTimer, this, &APlayerCharacter::PushBall,0.1f, true,0.0f);



	}	


	void APlayerCharacter::EndPushBallAction() 
	{
	
			GetWorldTimerManager().ClearTimer(PushBallActionTimer);


	}
	
	void APlayerCharacter::BeginPullBallAction() 
	{
	
		// get start timestamp
		// Set timer
		// play charge animation 

		/* RPC */
		if ( GetLocalRole() < ROLE_Authority )
		{
			ServerBeginPullBallAction();
		}

		PullBallActionTimestamp = GetWorld()->TimeSeconds;
		GetWorldTimerManager().SetTimer(PullBallActionTimer, this, &APlayerCharacter::PullBall,0.1f, true,0.0f);



	}	

	

	void APlayerCharacter::EndPullBallAction() 
	{
	
			GetWorldTimerManager().ClearTimer(PullBallActionTimer);


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



void APlayerCharacter::OnRep_PushBallAction()
{

	UE_LOG(LogTemp,Warning,TEXT("Replicate Action: Push Ball"));




}

void APlayerCharacter::OnRep_PullBallAction()
{

	UE_LOG(LogTemp,Warning,TEXT("Replicate Action: Pull Ball"));




}

void APlayerCharacter::OnRep_PrimaryAction()
{

	
	if ( CurrentWeapon == nullptr )
		return;





	//auto initDelay = 0;

	//initDelay = ( CurrentWeapon->InitialDelaySeconds > 0.0f ) ? CurrentWeapon->InitialDelaySeconds : CurrentWeapon->HitIntervalSeconds;


	GetWorldTimerManager().SetTimer(PrimaryActionTimer, this, &APlayerCharacter::AbortAction, 10.0f, false, 10.0f);

}

void  APlayerCharacter::OnRep_SecondaryAction()
{


}

// Called every frame


void APlayerCharacter::OnHealthChanged(UHealthComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{

	if ( Health <= 0.0f && !bDied) 
	{

		bDied = true;

		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}



}


void APlayerCharacter::PrimaryActionPressed()
{
		if ( CurrentWeapon == nullptr )
			return;


		if ( bPrimaryAction == true)
			return;

		bPrimaryAction = true;
		
		CurrentWeapon->Fire();

}

void APlayerCharacter::PrimaryActionReleased()
{
	bPrimaryAction = false;
	if ( CurrentWeapon != nullptr)
		CurrentWeapon->StopFire();	
	

}


void APlayerCharacter::SecondaryActionPressed() 
{


}
	

void APlayerCharacter::SecondaryActionReleased() 
{

}

void APlayerCharacter::AbortAction() 
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

	PlayerInputComponent->BindAction("ActionButton_A",EInputEvent::IE_Pressed,this,&APlayerCharacter::PrimaryActionPressed);
	PlayerInputComponent->BindAction("ActionButton_A",EInputEvent::IE_Released,this,&APlayerCharacter::PrimaryActionReleased);

	PlayerInputComponent->BindAction("ActionButton_B",EInputEvent::IE_Pressed,this,&APlayerCharacter::SecondaryActionPressed);
	PlayerInputComponent->BindAction("ActionButton_B",EInputEvent::IE_Released,this,&APlayerCharacter::SecondaryActionReleased);

	PlayerInputComponent->BindAction("ActionButton_C",EInputEvent::IE_Pressed,this,&APlayerCharacter::BeginPushBallAction);
	PlayerInputComponent->BindAction("ActionButton_C",EInputEvent::IE_Released,this,&APlayerCharacter::EndPushBallAction);

	PlayerInputComponent->BindAction("ActionButton_D",EInputEvent::IE_Pressed,this,&APlayerCharacter::BeginPullBallAction);
	PlayerInputComponent->BindAction("ActionButton_D",EInputEvent::IE_Released,this,&APlayerCharacter::EndPullBallAction);
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

void APlayerCharacter::ServerPushBall_Implementation() 
{
	PushBall();

}
bool APlayerCharacter::ServerPushBall_Validate() 
{

	return true;

}

void APlayerCharacter::PushBall() 
{

	/* RPC */
	if ( GetLocalRole() < ROLE_Authority )
	{
		ServerPushBall();
	}

	/* GET OWNER (Actor in possession of this weapon) */
	AActor* ActorOwner = GetOwner();
	if (ActorOwner != nullptr) 
	{ 
		/* LINE TRACE: CALCULATE START & STOP POSITIONS */
		FVector EyeLocation; FRotator EyeRotation;
		
		ActorOwner->GetActorEyesViewPoint(EyeLocation,EyeRotation);

		FVector A = GetActorLocation();
		FVector B = EyeLocation + (EyeRotation.Vector() * 10000);

		FVector ShotDirection = ( A != FVector(0) ) ? GetActorRotation().Vector() : EyeRotation.Vector();

		FVector TracerEndPoint = B;

		/* LINE TRACE: CONFIGURE PARAMETERS */
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(ActorOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		DrawDebugLine(GetWorld(),A,TracerEndPoint,FColor::White,false,1.0f,0,1.0f);	

		/* PERFORM LINE TRACE */
		FHitResult Hit;
		if ( GetWorld()->LineTraceSingleByChannel(Hit,A,B,ECC_GameTraceChannel1,QueryParams) ) 
		{

				TracerEndPoint = Hit.ImpactPoint;


				UStaticMeshComponent* _Mesh = Hit.GetActor()->FindComponentByClass<UStaticMeshComponent>();
				if (  _Mesh != nullptr )
				{
					
					_Mesh->AddImpulse(ShotDirection*10000.0f);

				}
			
		} else {
			EndPushBallAction();
			if ( GetLocalRole() == ROLE_Authority )
			{
				bPushBallAction = false;
			}
	
		}

		
	}

}


void APlayerCharacter::ServerPullBall_Implementation() 
{
	PullBall();

}
bool APlayerCharacter::ServerPullBall_Validate() 
{

	return true;

}

void APlayerCharacter::PullBall() 
{

	/* RPC */
	if ( GetLocalRole() < ROLE_Authority )
	{
		ServerPullBall();
	}

	/* GET OWNER (Actor in possession of this weapon) */
	AActor* ActorOwner = GetOwner();
	if (ActorOwner != nullptr) 
	{ 
		/* LINE TRACE: CALCULATE START & STOP POSITIONS */
		FVector EyeLocation; FRotator EyeRotation;
		
		ActorOwner->GetActorEyesViewPoint(EyeLocation,EyeRotation);

		FVector A = GetActorLocation();
		FVector B = EyeLocation + (EyeRotation.Vector() * 10000);

		FVector ShotDirection = ( A != FVector(0) ) ? GetActorRotation().Vector() : EyeRotation.Vector();

		FVector TracerEndPoint = B;

		/* LINE TRACE: CONFIGURE PARAMETERS */
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(ActorOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		DrawDebugLine(GetWorld(),A,TracerEndPoint,FColor::White,false,1.0f,0,1.0f);	

		/* PERFORM LINE TRACE */
		FHitResult Hit;
		if ( GetWorld()->LineTraceSingleByChannel(Hit,A,B,ECC_GameTraceChannel1,QueryParams) ) 
		{

				TracerEndPoint = Hit.ImpactPoint;


				UStaticMeshComponent* _Mesh = Hit.GetActor()->FindComponentByClass<UStaticMeshComponent>();
				if (  _Mesh != nullptr )
				{
					
					_Mesh->AddImpulse(-ShotDirection*10000.0f);

				}
			
		} else {
			EndPullBallAction();
			if ( GetLocalRole() == ROLE_Authority )
			{
				bPullBallAction = false;
			}
	
		}

		
	}

}