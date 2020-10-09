#include "BasketBall.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerState.h"
#include "HealthComponent.h"
#include "PlayerCharacterController.h"
#include "PowerBallGameState.h"

#define DISTANCE_IN_FRONT_OF_POSSESSOR		90.f
#define POSSESSOR_Z_OFFSET					-60.f

ABasketBall::ABasketBall(const class FObjectInitializer& OI)
	: Super(OI)
{
	Possessor = nullptr;
	LastPossessor = nullptr;
	PossessorToIgnore = nullptr;
	NegDistanceTravelled = 0.0f;
	proxyStateCount = 0;
	this->SetActorTickEnabled(true);
	PrimaryActorTick.bCanEverTick = true;
	serverChargeBeginTime = -1;
	IsCharging = false;
	ThisBasketBall = this;
}

void ABasketBall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{


	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	DOREPLIFETIME(ABasketBall, Possessor);
	DOREPLIFETIME(ABasketBall, LastPossessor);
	DOREPLIFETIME(ABasketBall, ServerPhysicsState);
}

void ABasketBall::OnRep_ServerPhysicsState()
{
	// If we get here, we are always the client. Here we store the physics state
	// for physics state interpolation.

	// Shift the buffer sideways, deleting state PROXY_STATE_ARRAY_SIZE
	for (int i = PROXY_STATE_ARRAY_SIZE - 1; i >= 1; i--)
	{
		proxyStates[i] = proxyStates[i - 1];
	}

	// Record current state in slot 0
	proxyStates[0] = ServerPhysicsState;

	// Update used slot count, however never exceed the buffer size
	// Slots aren't actually freed so this just makes sure the buffer is
	// filled up and that uninitalized slots aren't used.
	proxyStateCount = FMath::Min(proxyStateCount + 1, PROXY_STATE_ARRAY_SIZE);

	// Check if states are in order
	if (proxyStates[0].timestamp < proxyStates[1].timestamp)
	{ 
		UE_LOG(LogNetSerialization, Verbose, TEXT("Timestamp inconsistent: %d should be greater than %d"), proxyStates[0].timestamp, proxyStates[1].timestamp);
	}		
}

/** Simulates the free movement of the ball based on proxy states */
void ABasketBall::ClientSimulateFreeMovingBall()
{
	
	APlayerCharacterController* MyPC = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (nullptr == MyPC || 0 == proxyStateCount)
	{
		// We don't know yet know what the time is on the server yet so the timestamps
		// of the proxy states mean nothing; that or we simply don't have any proxy
		// states yet. Don't do any interpolation.
		SetActorLocationAndRotation(ServerPhysicsState.pos, ServerPhysicsState.rot);
	}
	else
	{
		float interpolationBackTime = 100;
		float extrapolationLimit = 500;

		// This is the target playback time of the rigid body
		float interpolationTime = GetGameState()->GetServerWorldTimeSeconds() - interpolationBackTime;

		// Use interpolation if the target playback time is present in the buffer
		if (proxyStates[0].timestamp > interpolationTime)
		{
			// Go through buffer and find correct state to play back
			for (int i=0;i<proxyStateCount;i++)
			{
				if (proxyStates[i].timestamp <= interpolationTime || i == proxyStateCount-1)
				{
					// The state one slot newer (<100ms) than the best playback state
					FSmoothPhysicsState rhs = proxyStates[FMath::Max(i - 1, 0)];
					// The best playback state (closest to 100 ms old (default time))
					FSmoothPhysicsState lhs = proxyStates[i];

					// Use the time between the two slots to determine if interpolation is necessary
					int64 length = (int64)(rhs.timestamp - lhs.timestamp);
					double t = 0.0F;
					// As the time difference gets closer to 100 ms t gets closer to 1 in
					// which case rhs is only used
					if (length > 1)
						t = (double)(interpolationTime - lhs.timestamp) / (double)length;

					// if t=0 => lhs is used directly					
					FVector pos = FMath::Lerp(lhs.pos, rhs.pos, t);
					FRotator rot = FMath::Lerp(lhs.rot, rhs.rot, t);
					SetActorLocationAndRotation(pos, rot);
					return;
				}
			}
		}
		// Use extrapolation
		else
		{
			FSmoothPhysicsState latest = proxyStates[0];

			float extrapolationLength = interpolationTime - latest.timestamp;
			// Don't extrapolate for more than [extrapolationLimit] milliseconds
			if (extrapolationLength < extrapolationLimit)
			{
				FVector pos = latest.pos + latest.vel * ((float)extrapolationLength * 0.001f);
				FRotator rot = latest.rot;
				SetActorLocationAndRotation(pos, rot);
			}
			else
			{
				// Don't move. If we're this far away from the server, we must be pretty laggy.
				// Wait to catch up with the server.
			}
		}
	}

	
}

/** This occurs when play begins */
void ABasketBall::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() < ROLE_Authority)
	{
		// The server manages the game state; the soccer ball will be replicated to us.

		// Physics however are not replicated. We will need to have the ball orientation
		// replicated to us. We need to turn off physics simulation and collision detection.
		UPrimitiveComponent *Root = Cast<UPrimitiveComponent>(GetRootComponent());
		Root->PutRigidBodyToSleep();
		Root->SetSimulatePhysics(false);
		Root->SetEnableGravity(false);
		SetActorEnableCollision(false);
	}
	else
	{
		// Servers should add this soccer ball to the game mode cache.
		// It will get replicated to clients for when they need to access
		// the ball itself to get information such as who possesses it.

		auto g = GetWorld()->GetGameState<APowerBallGameState>();

		if ( g != nullptr && ThisBasketBall != nullptr)
		{

			g->SetBasketBall(ThisBasketBall);

		}
	}
}


APlayerCharacter* ABasketBall::GetPossessor() {
	return ( Possessor != nullptr ) ? Possessor : nullptr;
}


void ABasketBall::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if ( !RoundHasStarted() )
		return;

	if (nullptr != Possessor)
	{
		// If the ball is possessed by a player, then the ball should move as a function
		// of the possessor
		MoveWithPossessor();
	}
	else
	{
		// No possessor. The ball is freely moving.
		if (GetLocalRole()  < ROLE_Authority)
		{
			// Clients should update its local position based on where it is on the server
			ClientSimulateFreeMovingBall();
		}
		else
		{
			// Update the charging value
			if (GetVelocity().SizeSquared() > 1.f)
			{
				IsCharging = false;
			}

			// Servers should simulate the ball physics freely and replicate the orientation
			UPrimitiveComponent *Root = Cast<UPrimitiveComponent>(GetRootComponent());
			ServerPhysicsState.pos = GetActorLocation();
			ServerPhysicsState.rot = GetActorRotation();
			ServerPhysicsState.vel = GetVelocity();
			ServerPhysicsState.timestamp = GetGameState()->GetServerWorldTimeSeconds();

			// Servers should also test the distance from the old possessor and reset it
			if (nullptr != PossessorToIgnore)
			{

				
				if (PossessorToIgnore->bDied)
				{
					PossessorToIgnore = nullptr;
				}
				else
				{

					
					FVector2D PossessorLoc(PossessorToIgnore->GetActorLocation());
					FVector2D BallLoc(GetActorLocation());
					float d = FVector2D::DistSquared(PossessorLoc, BallLoc);
					if (d > 14000)
					{
						PossessorToIgnore = nullptr;
					}
				}
			}
		}
	}
}

/** Called by the GameMode object when the next round has begun */
void ABasketBall::RoundHasStarted_Implementation()
{
	if (GetLocalRole()  == ROLE_Authority)
	{
		if (nullptr != GetPossessor())
		{
			SetPossessor(nullptr); // This change will be replicated to all clients
		}
		else
		{
			UPrimitiveComponent *Root = Cast<UPrimitiveComponent>(GetRootComponent());
			Root->PutRigidBodyToSleep();
		}
		PossessorToIgnore = nullptr;
	}
}

/** Called by a APlayerCharacter object when it has been destroyed */
void ABasketBall::CharacterHasDestroyed_Implementation(APlayerCharacter *Character)
{
	if (nullptr != Character && GetLocalRole()  == ROLE_Authority)
	{
		// Release the ball and ensure the possessor to ignore is reset
		if (Character->PossessesBall())
		{
			SetPossessor(nullptr);
			PossessorToIgnore = nullptr;
		}

		if (nullptr != LastPossessor && LastPossessor->GetUniqueID() == Character->GetUniqueID())
		{
			LastPossessor = nullptr;
		}
	}
}

/** Gets the game state */
APowerBallGameState* ABasketBall::GetGameState()
{
	APowerBallGameState* state = GetWorld()->GetGameState<APowerBallGameState>();
	return state;
}

/** True if the ball has no possessor and is not in a goal. Should only be called by the authority entity. */
bool ABasketBall::IsFree()
{

	return (nullptr == GetPossessor());
}

/** True if the player can possess the ball */
bool ABasketBall::CanPossessBall(APlayerCharacter* Player)
{
	return (nullptr != Player && (Player->HealthComponent->Health > 0.f));
}

void ABasketBall::MoveWithPossessor()
{
	// This is used to roll the ball while a possessor owns it. The angle is a function of how far the ball has travelled.
	NegDistanceTravelled += GetPossessor()->GetVelocity().Size() * -0.015f;

	// All platforms should move the same way if there is a possessor.
	//SetActorLocationAndRotation(Possessor->GetActorLocation() + Possessor->GetActorForwardVector() * DISTANCE_IN_FRONT_OF_POSSESSOR + FVector(0.0f, 0.0f, POSSESSOR_Z_OFFSET),
	//	FRotator(NegDistanceTravelled, Possessor->GetActorRotation().Yaw, 0.0f));
	SetActorLocationAndRotation(GetPossessor()->GetMesh()->GetSocketLocation(FName("BallSocket")),FRotator(0,0,0));

}

/** Sets the current ball possessor */
void ABasketBall::SetPossessor(APlayerCharacter* Player)
{
	if (GetLocalRole()  < ROLE_Authority)
	{
		// Safety check. Only authority entities should drive the ball.
	}
	else
	{
		// We only allow a possession change if the ball is being unpossessed or the player is not one we're ignoring
		if (nullptr == Player || (CanPossessBall(Player) && (PossessorToIgnore != Player)))
		{
			APlayerCharacter *OldPossessor = GetPossessor();

			// Assign the new possessor
			Possessor = Player;
			
			// Handle cases when the ball had a possessor in the previous frame
			if (nullptr != OldPossessor)
			{
				// Assign the last possessor
				LastPossessor = OldPossessor;
				// Assign the possessor to ignore
				PossessorToIgnore = OldPossessor;
			}

			// Toggle physics
			UPrimitiveComponent *Root = Cast<UPrimitiveComponent>(GetRootComponent());
			if (nullptr != GetPossessor())
			{
//				Possessor->StopWeaponFire(Possessor->PrimaryWeapon);
//				Possessor->StopWeaponFire(Possessor->SecondaryWeapon);
				Root->PutRigidBodyToSleep();
				Root->SetSimulatePhysics(false);
				Root->SetEnableGravity(false);
				SetActorEnableCollision(false);
				MoveWithPossessor();
			}
			else
			{
				Root->SetSimulatePhysics(true);
				Root->SetEnableGravity(true);
				SetActorEnableCollision(true);
				Root->PutRigidBodyToSleep();
			}
		}

		// Force the orientation to be replicated at the same time the possessor is replicated
		UPrimitiveComponent *Root = Cast<UPrimitiveComponent>(GetRootComponent());
		ServerPhysicsState.pos = GetActorLocation();
		ServerPhysicsState.rot = GetActorRotation();
		ServerPhysicsState.vel = GetVelocity();
		ServerPhysicsState.timestamp = GetGameState()->GetServerWorldTimeSeconds();
	}
}

/** Kicks this ball with a given force */
void ABasketBall::Throw(const FVector& Force)
{
	if (nullptr == GetPossessor())
	{
		// Safety check. The possessor must be valid.
	}
	else
	{
		// Reset the possessor
		SetPossessor(nullptr);

		// Now apply the force
		UPrimitiveComponent *Root = Cast<UPrimitiveComponent>(GetRootComponent());
		Root->AddImpulse(Force);
	}
}

/** Kicks this ball to a location */
void ABasketBall::ThrowToLocation(const FVector& Location, float AngleInDegrees)
{
	if (nullptr == GetPossessor())
	{
		// Safety check. The possessor must be valid.
	}
	else
	{
		// Reset the possessor
		SetPossessor(nullptr);

		// Calculate the angle required to hit the coordinate (x,y) on the plane containing
		// the origin and ground point. The x coordinate is the distance from the ball
		// to where the user clicked, and the y coordinate should be zero.
		// http://en.wikipedia.org/wiki/Trajectory_of_a_projectile

		FVector Origin = GetActorLocation();
		float x = FVector::Dist(Location, Origin);
		float y = 0.f;
		float g = -980.f; // Standard gravity
		float r = AngleInDegrees * 3.14159f / 180.f;
		float p = std::tan(r);
		float v = std::sqrt((p*p + 1.f)*(g*x*x) / (2.f * (y - x*p)));
		float forceMag = v;

		FVector forward = (Location - Origin);
		forward.Z = 0; // We only support kicking to locations that are on the ball's Z plane
		forward.Normalize();
		FVector cross = FVector::CrossProduct(forward, FVector::UpVector);
		FVector kick = forward * FMath::Cos(r) * forceMag;
		kick.Z = FMath::Sin(r) * forceMag;

		UPrimitiveComponent *Root = Cast<UPrimitiveComponent>(GetRootComponent());
		Root->SetPhysicsLinearVelocity(kick);
		Root->SetPhysicsAngularVelocityInDegrees(cross * forceMag * -4.f);
	}
}

/** [server] begins charging the ball */
void ABasketBall::BeginCharging()
{
	// Start charging
	serverChargeBeginTime = GetGameState()->GetServerWorldTimeSeconds();
	IsCharging = true;
}