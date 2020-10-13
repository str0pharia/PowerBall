#pragma once

#include "GameFramework/Actor.h"
#include "Engine/StaticMeshActor.h"
#include "BasketBall.generated.h"

class APlayerCharacter;
class APowerBallGameState;
class UStaticMeshComponent;


UCLASS()
class POWERBALL_API ABasketBall : public AStaticMeshActor
{
	GENERATED_BODY()

		ABasketBall();
	
	public:

    	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "PowerBall")
    	APlayerCharacter* Possessor;

    	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "PowerBall")
    	APlayerCharacter* LastPossessor;

 		UFUNCTION(Server, Reliable, WithValidation)
		void ServerPossess(APlayerCharacter* Player);

		UFUNCTION()
		void Possess(APlayerCharacter* Player);

		UFUNCTION()
    	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
     	
		void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

		/** Authorative */
		UFUNCTION(Category = "PowerBall")
		bool IsFree();

	protected:

    	virtual void BeginPlay() override;

		UStaticMeshComponent* BallMesh;
			


};
