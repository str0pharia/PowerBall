// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = WeaponMesh;

	EffectOriginSocketName = FName("Source");

	SetReplicates(true);
}

// Cast/Trigger Spell
void AWeapon::Fire() 
{
	/* RPC */
	if ( GetLocalRole() < ROLE_Authority )
	{
		ServerFire();
	}

	/* GET OWNER (Actor in possession of this weapon) */
	AActor* ActorOwner = GetOwner();
	if (ActorOwner != nullptr) 
	{ 

		/* LINE TRACE: CALCULATE START & STOP POSITIONS */
		FVector EyeLocation;
		FRotator EyeRotation;
		ActorOwner->GetActorEyesViewPoint(EyeLocation,EyeRotation);
		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 10000);
		FVector TracerEndPoint = TraceEnd;

		/* LINE TRACE: CONFIGURE PARAMETERS */
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(ActorOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		/* PERFORM LINE TRACE */
		FHitResult Hit;
		if ( GetWorld()->LineTraceSingleByChannel(Hit,EyeLocation,TraceEnd,ECC_GameTraceChannel1,QueryParams) ) 
		{

				/* ON HIT */
				AActor* HitActor = Hit.GetActor();

				/* APPLY DAMAGE */	
				UGameplayStatics::ApplyPointDamage(HitActor,20.0f,ShotDirection,Hit,ActorOwner->GetInstigatorController(),this,DamageType);
				
				/* SELECT IMPACT EFFECT BASED ON SURFACE TYPE */ 
				UParticleSystem* SelectedEffect = nullptr;
				EPhysicalSurface SurfaceType  = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
				switch (SurfaceType)
				{
					// Flesh
					case SurfaceType1:
							SelectedEffect = FleshImpactEffect;
						break;
					// Flesh Critical 
					case SurfaceType2:
							SelectedEffect = FleshCriticalImpactEffect;
						break;
					default:
							SelectedEffect = DefaultImpactEffect;	
						break;

				}

				/* SPAWN IMPACT EFFECT */
				if (SelectedEffect)
				{					
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),SelectedEffect,Hit.ImpactPoint,Hit.ImpactNormal.Rotation());
				}

				TracerEndPoint = Hit.ImpactPoint;
		}


		DrawDebugLine(GetWorld(),EyeLocation,TraceEnd,FColor::White,false,1.0f,0,1.0f);	

		/* SPAWN TRIGGER AND/OR TRACER EFFECT */
		SpawnEffects(TracerEndPoint);

	}

}

// Spawn Magic Effect Emitters
void AWeapon::SpawnEffects(FVector TraceEnd) 
{

		/* TRIGGER EFFECT */
		if ( TriggerEffect )
		{
			UGameplayStatics::SpawnEmitterAttached(TriggerEffect,WeaponMesh,EffectOriginSocketName);
		}

		/* TRACER EFFECT */
		if ( TracerEffect ) 
		{
			FVector SourceLocation = WeaponMesh->GetSocketLocation(EffectOriginSocketName);
			UParticleSystemComponent* TraceEffectInstance = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),TracerEffect,SourceLocation);
			if ( TraceEffectInstance ) 
			{
				TraceEffectInstance->SetVectorParameter("Target",TraceEnd);
			}	
		}
}

/* Magic Attacl RPC */
void AWeapon::ServerFire_Implementation() 
{
	Fire();
}

bool AWeapon::ServerFire_Validate() 
{
	return true;
}