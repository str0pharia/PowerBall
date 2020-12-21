#pragma once

#include "GameFramework/Actor.h"
#include "Engine/StaticMeshActor.h"
#include "BasketBall.generated.h"

class APlayerCharacter;
class APowerBallGameState;
class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class POWERBALL_API ABasketBall : public AStaticMeshActor
{
	GENERATED_BODY()

	ABasketBall();
	
	

	protected:

    	//virtual void BeginPlay();

		UStaticMeshComponent* BallMesh;
			
	//		USphereComponent* PickUpSphere = nullptr;

};
