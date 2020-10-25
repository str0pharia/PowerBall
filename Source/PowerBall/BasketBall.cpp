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

		PrimaryActorTick.bCanEverTick = true;

		/*
	PickUpSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickUpcollision"));

	PickUpSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickUpSphere->SetGenerateOverlapEvents(true);

	PickUpSphere->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	PickUpSphere->SetCollisionResponseToChannel(ECC_GameTraceChannel3,ECR_Overlap);
	PickUpSphere->SetSphereRadius(145.0f);

	
	PickUpSphere->OnComponentBeginOverlap.AddDynamic(this,&ABasketBall::OnOverlapBegin);
	
*/
	
	BallMesh = FindComponentByClass<UStaticMeshComponent>();

}

 void ABasketBall::BeginPlay() 
 {
	 /*
 		Possessor = nullptr;
	 	LastPossessor = nullptr;
*/
		(GetWorld()->GetGameState<APowerBallGameState>())->SetBasketBall(this);

	
 }
 

void ABasketBall::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	/*
	
		if ( nullptr != Possessor ) {

			MoveWithPossessor();
		
		}
	
	*/
	
}


void ABasketBall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{


	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	//DOREPLIFETIME(ABasketBall, Possessor);
	//DOREPLIFETIME(ABasketBall, LastPossessor);

}


/*
void ABasketBall::Possess(APlayerCharacter* Player) 
{

	
	if ( GetLocalRole() < ROLE_Authority )
	{
		ServerPossess(Player);

	}




	LastPossessor = Possessor;


	Possessor = Player;

	BallMesh->SetSimulatePhysics(false);
	PickUpSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachToComponent(Player->GetPlayerMesh(),FAttachmentTransformRules::KeepWorldTransform,FName("BallSocket"));

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


	if ( OtherActor->ActorHasTag(TEXT("Player")))  
	{

		if ( IsFree() ) {
			APlayerCharacter* Player = (APlayerCharacter*)OtherActor->GetInstigator();
			if ( Player != nullptr ) {
				Possess(Player);
			}
		}
	}
}


bool ABasketBall::IsFree()
{
	return ( Possessor == nullptr);
}

void ABasketBall::MoveWithPossessor() 
{
	FHitResult Hit;
	GetRootComponent()->SetWorldLocation(Possessor->GetActorLocation() + Possessor->GetBallSocketLocation() , false, &Hit, ETeleportType::TeleportPhysics);

}

void ABasketBall::Launch() 
{

	if ( GetLocalRole() < ROLE_Authority )
	{

		ServerLaunch();

		
	} 

		


	
		BallMesh->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld,EDetachmentRule::KeepWorld,EDetachmentRule::KeepWorld,true));
	
		PickUpSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		BallMesh->SetSimulatePhysics(true);
		BallMesh->SetSimulatePhysics(true);
		SetActorLocationAndRotation(Possessor->GetBallSocketLocation(),Possessor->GetActorForwardVector().Rotation());


			LastPossessor = Possessor;
			Possessor = nullptr;

}


void ABasketBall::ServerLaunch_Implementation() 
{


	Launch();
}

bool ABasketBall::ServerLaunch_Validate()  
{ 
	return true; 
}


*/
