// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicNinjaBlade.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

AMagicNinjaBlade::AMagicNinjaBlade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EffectOriginSocketName = FName("Source");

	SetReplicates(true);
}

void AMagicNinjaBlade::SpawnEffects(FVector TraceEnd) 
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

void AMagicNinjaBlade::Fire()
{


    ServerFire();
	
  
}

void AMagicNinjaBlade::ServerFire()
{
	GetWorldTimerManager().SetTimer(HitTimer, this, &AMagicNinjaBlade::Hit, 1.0f, true, 2.0f);



}

void AMagicNinjaBlade::Hit()
{
    
    /* GET OWNER (Actor in possession of this weapon) */
	AActor* ActorOwner = GetOwner();
	if (ActorOwner != nullptr) 
	{ 
        HitCounter++; 
		/* LINE TRACE: CALCULATE START & STOP POSITIONS */
		FVector EyeLocation; FRotator EyeRotation;
		
		ActorOwner->GetActorEyesViewPoint(EyeLocation,EyeRotation);

		FVector A = WeaponMesh->GetSocketLocation(EffectOriginSocketName);
		FVector B = EyeLocation + (EyeRotation.Vector() * 10000);

		FVector ShotDirection = ( A != FVector(0) ) ? WeaponMesh->GetSocketRotation(EffectOriginSocketName).Vector() : EyeRotation.Vector();

		FVector TracerEndPoint = B;

		/* LINE TRACE: CONFIGURE PARAMETERS */
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(ActorOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		/* PERFORM LINE TRACE */
		FHitResult Hit;
		if ( GetWorld()->LineTraceSingleByChannel(Hit,A,B,ECC_GameTraceChannel1,QueryParams) ) 
		{

				/* ON HIT */
				AActor* HitActor = Hit.GetActor();

				/* APPLY DAMAGE */	
				UGameplayStatics::ApplyPointDamage(HitActor,Damage,ShotDirection,Hit,ActorOwner->GetInstigatorController(),this,DamageType);
				
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


		DrawDebugLine(GetWorld(),A,TracerEndPoint,FColor::White,false,1.0f,0,1.0f);	

		/* SPAWN TRIGGER AND/OR TRACER EFFECT */
		SpawnEffects(TracerEndPoint);

		if ( GetLocalRole() == ROLE_Authority) 
		{
			HitScanTrace.TraceTo = TracerEndPoint;
		}

		LastFireTime = GetWorld()->TimeSeconds;
	}

    if ( HitTimer.IsValid() && HitCounter >= 3 )
    {

        GetWorldTimerManager().ClearTimer(HitTimer);
        HitCounter = 0;
    }

}