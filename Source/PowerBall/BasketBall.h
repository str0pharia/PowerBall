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
	
	public:

    	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "PowerBall")
    	APlayerCharacter* Possessor;

    	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "PowerBall")
    	APlayerCharacter* LastPossessor;

 		UFUNCTION(Server, Reliable, WithValidation)
		void ServerPossess(APlayerCharacter* Player);

		UFUNCTION()
		void Possess(APlayerCharacter* Player);

		float NegDistanceTraveled;

		UFUNCTION()
    	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent*  OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
     	
		UFUNCTION()
		void OnOverlapEnd( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent*  OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
		
		void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

		UFUNCTION(Category = "PowerBall")
		bool IsFree();

		void Launch();

		UFUNCTION(Server, Reliable, WithValidation) 
		void ServerLaunch();

		void Eject();

		UFUNCTION(Server, Reliable, WithValidation) 
		void ServerEject();

		UFUNCTION()
		void MoveWithPossessor();

	protected:

    	virtual void BeginPlay() override;

		virtual void Tick(float DeltaTime) override;

		UStaticMeshComponent* BallMesh;
			
		USphereComponent* PickUpSphere;

};
