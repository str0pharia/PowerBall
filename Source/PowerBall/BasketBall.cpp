#include "BasketBall.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerState.h"
#include "HealthComponent.h"
#include "PlayerCharacterController.h"
#include "PowerBallGameState.h"
#include "Components/SphereComponent.h"


ABasketBall::ABasketBall()
{


	
	PickUpSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickUpcollision"));


	PickUpSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickUpSphere->SetGenerateOverlapEvents(true);

	PickUpSphere->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	PickUpSphere->SetCollisionResponseToChannel(ECC_GameTraceChannel3,ECR_Overlap);
	PickUpSphere->SetSphereRadius(145.0f);
	PickUpSphere->OnComponentBeginOverlap.AddDynamic(this,&ABasketBall::OnOverlapBegin);
	

}

 void ABasketBall::BeginPlay() 
 {
	 Possessor = nullptr;
	 LastPossessor = nullptr;

 }


void ABasketBall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{


	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	DOREPLIFETIME(ABasketBall, Possessor);
	DOREPLIFETIME(ABasketBall, LastPossessor);


}


void ABasketBall::Possess(APlayerCharacter* Player) 
{


	AttachToActor(Player->GetOwner(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName("BallSocket"));

}	

void ABasketBall::ServerPossess_Implementation(APlayerCharacter* Player) 
{

	Possess(Player);

}


bool ABasketBall::ServerPossess_Validate(APlayerCharacter* Player) 
{
	return true;
}

void ABasketBall::OnOverlapBegin( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent*  OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	if ( IsFree() && OtherActor->ActorHasTag(FName("Player")) ) 
	{

		UE_LOG(LogTemp,Warning,TEXT("HASDSAD"));
		APlayerCharacter* Player = (APlayerCharacter*)OtherActor->GetInstigator();
		if ( Player != nullptr )
			Possess(Player);


	}

}


bool ABasketBall::IsFree()
{

	return ( Possessor == nullptr);

}

