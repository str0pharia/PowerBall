// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileWeapon.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "Projectile.h"
#include "Weapon.h"




	void AProjectileWeapon::Fire() 
    {
        	/* RPC */


        if (GetLocalRole() == ROLE_Authority)
            SpawnProjectile();

    }

	void AProjectileWeapon::SpawnEffects(FVector TraceEnd)
    {


    }

	void AProjectileWeapon::SpawnProjectile_Implementation()
    {

        if ( ProjectileActor == nullptr)
            return;
            
            
            FActorSpawnParameters SpawnInfo;
            AProjectile *a = GetWorld()->SpawnActor<AProjectile>(GetActorLocation(), GetActorRotation(), SpawnInfo);
            if ( a != nullptr) {

                UE_LOG(LogTemp,Warning,TEXT("Spawned"));
            }



    }
