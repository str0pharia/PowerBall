#pragma once

#include "GameFramework/Actor.h"
#include "SmoothPhysicsState.h"
#include "BasketBall.generated.h"

#define PROXY_STATE_ARRAY_SIZE 20

class APlayerCharacter;
class APowerBallGameState;

UCLASS()
class POWERBALL_API ABasketBall : public AActor
{
	GENERATED_UCLASS_BODY()

	public:
	/** The player that possesses this soccer ball */
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "PowerBall")
	APlayerCharacter* Possessor;

	/** The player who last possessed this soccer ball */
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "PowerBall")
	APlayerCharacter* LastPossessor;

	UFUNCTION(BlueprintCallable)
	APlayerCharacter* GetPossessor();

	/** The soccer ball orientation on the server */
	UPROPERTY(ReplicatedUsing = OnRep_ServerPhysicsState)
	FSmoothPhysicsState ServerPhysicsState;

	UFUNCTION()
	void OnRep_ServerPhysicsState();

	/** The negative distance travelled since creation. Maintained locally. */
	float NegDistanceTravelled;

	/** [server + local] True if the ball is charging */
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "PowerBall")
	bool IsCharging;

	/** [server] The time when the ball began charging */
	float serverChargeBeginTime;

private:
	/** Clients store twenty states with "playback" information from the server. This
	array contains the official state of this object at different times according to
	the server. */
	FSmoothPhysicsState proxyStates[PROXY_STATE_ARRAY_SIZE];

	/** Keep track of what slots are used */
	int proxyStateCount;

private:
	/** Simulates the free movement of the ball based on proxy states */
	void ClientSimulateFreeMovingBall();

public:
	//Begin AActor interface

	/** This occurs when play begins */
	virtual void BeginPlay() override;

	void Tick(float DeltaSeconds) override;

	//End AActor interface

	/** Called by the GameMode object when the next round has begun */
	UFUNCTION(BlueprintNativeEvent, Category = "PowerBall")
	void RoundHasStarted();

	/** Called by a character object when it has been destroyed */
	UFUNCTION(BlueprintNativeEvent, Category = "PowerBall")
	void CharacterHasDestroyed(APlayerCharacter *Character);

	/** Gets the game state */
	APowerBallGameState* GetGameState();

	/** True if the ball has no possessor and is not in a goal. Should only be called by the authority entity. */
	UFUNCTION(BlueprintCallable, Category = "PowerBall")
	bool IsFree();

	/** True if the player can possess the ball */
	bool CanPossessBall(APlayerCharacter* Player);

	/** When a possessor releases the ball, we don't want to allow them to repossess the ball until it has
	travelled a certain distance away. This variable lets the ball remember who possessed the ball last so
	it can do this calculation. Once the distance has been exceeded, this is set to null. */
	class APlayerCharacter *PossessorToIgnore;

	/** Sets the current ball possessor */
	void SetPossessor(class APlayerCharacter* Player);

	/** Move with the possessing player */
	void MoveWithPossessor();

	/** Kicks this ball with a given force */
	void Throw(const FVector& Force);

	/** Kicks this ball to a location */
	void ThrowToLocation(const FVector& Location, float AngleInDegrees);

	/** [server] begins charging the ball */
	void BeginCharging();

	ABasketBall* ThisBasketBall = nullptr;
};
