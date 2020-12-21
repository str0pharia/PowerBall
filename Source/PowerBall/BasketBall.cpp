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


 