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

void AWeapon::Fire() 
{
	if ( GetLocalRole() < ROLE_Authority )
	{

		ServerFire();

	}

	AActor* ActorOwner = GetOwner();
	if (ActorOwner != nullptr) 
	{ 
		FVector EyeLocation;
		FRotator EyeRotation;

		ActorOwner->GetActorEyesViewPoint(EyeLocation,EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 10000);


		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(ActorOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		FVector TracerEndPoint = TraceEnd;
		FHitResult Hit;
		if ( GetWorld()->LineTraceSingleByChannel(Hit,EyeLocation,TraceEnd,ECC_GameTraceChannel1,QueryParams) ) 
		{

				// On Hit

				AActor* HitActor = Hit.GetActor();
				UGameplayStatics::ApplyPointDamage(HitActor,20.0f,ShotDirection,Hit,ActorOwner->GetInstigatorController(),this,DamageType);

				EPhysicalSurface SurfaceType  = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
				UParticleSystem* SelectedEffect = nullptr;
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

				
				if (SelectedEffect)
				{
					
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),SelectedEffect,Hit.ImpactPoint,Hit.ImpactNormal.Rotation());


				}

				TracerEndPoint = Hit.ImpactPoint;


		}


		DrawDebugLine(GetWorld(),EyeLocation,TraceEnd,FColor::White,false,1.0f,0,1.0f);	

		SpawnEffects(TracerEndPoint);

	}

}

void AWeapon::SpawnEffects(FVector TraceEnd) 
{

		if ( TriggerEffect )
		{

			UGameplayStatics::SpawnEmitterAttached(TriggerEffect,WeaponMesh,EffectOriginSocketName);

		}

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

void AWeapon::ServerFire_Implementation() 
{
	Fire();
}

bool AWeapon::ServerFire_Validate() 
{
	return true;
}