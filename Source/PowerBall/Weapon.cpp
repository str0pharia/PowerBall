// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

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

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Fire() 
{

	AActor* ActorOwner = AActor::GetOwner();

	if (ActorOwner) 
	{ 
		FVector EyeLocation;
		FRotator EyeRotation;

		ActorOwner->GetActorEyesViewPoint(EyeLocation,EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 10000);
		FVector TracerEndPoint = TraceEnd;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(ActorOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;

		FHitResult Hit;
		if ( GetWorld()->LineTraceSingleByChannel(Hit,EyeLocation,TraceEnd,ECC_GameTraceChannel1,QueryParams) ) 
		{

				// On Hit

				AActor* HitActor = Hit.GetActor();

				UGameplayStatics::ApplyPointDamage(HitActor,20.0f,ShotDirection,Hit,ActorOwner->GetInstigatorController(),this,DamageType);


				if ( ImpactEffect)
				{
					
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactEffect,Hit.ImpactPoint,Hit.ImpactNormal.Rotation());


				}


				TracerEndPoint = Hit.ImpactPoint;


		}


		DrawDebugLine(GetWorld(),EyeLocation,TraceEnd,FColor::White,false,1.0f,0,1.0f);	

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
				
				TraceEffectInstance->SetVectorParameter("Target",TracerEndPoint);

			}
		}

	}



}

