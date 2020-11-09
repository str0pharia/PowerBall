// Fill out your copyright notice in the Description page of Project Settings.


#include "GiantHand.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "DrawDebugHelpers.h"
#include "Components/SplineComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Hand.h"

    AGiantHand::AGiantHand() 
    {

        PrimaryActorTick.bCanEverTick = true;

	    EffectOriginSocketName = FName("Source");

        HitIntervalSeconds = 0.1f;

        Duration = 4.0f;

        static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/Blueprints/Summon/Hand/HandSummon'"));
        if (ItemBlueprint.Object)
        {
            HandTemplate = (UClass*)ItemBlueprint.Object->GeneratedClass;
        }

        SetReplicates(true);

    }

    void AGiantHand::Fire()
    {
        ServerFire();
    }

    void AGiantHand::ServerFire()
    {      
        UE_LOG(LogTemp,Warning,TEXT("ServerFire"));
        LastTraceHitTimeStamp = -1.0f;
        StartFireTime = GetWorld()->TimeSeconds;
        LockedTarget = nullptr;
    }


    void AGiantHand::StopFire() 
    {   
        //TraceTarget();
        ServerStopFire();
    }

    // Cast/Trigger Spell
    void AGiantHand::ServerStopFire() 
    {
        UE_LOG(LogTemp,Warning,TEXT("ServerStopFire"));
        //GetWorldTimerManager().ClearTimer(PrimaryActionTimer);
        if ( HandObjInstance != nullptr)
        {
            return;
        }
        
        FVector EyeLocation; FRotator EyeRotation;
                
        GetOwner()->GetActorEyesViewPoint(EyeLocation,EyeRotation);

        FVector SpawnPoint = EyeLocation + (GetOwner()->GetActorForwardVector() * 20.0);
        FVector ShotDirection = EyeRotation.Vector() * 1.f;

        FActorSpawnParameters params;

        params.Owner = GetOwner();
        params.Instigator = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
        bool bHaveAimSolution = false;
        FVector V;
        HandObjInstance = GetWorld()->SpawnActor<AHand>(HandTemplate,SpawnPoint,ShotDirection.Rotation(),params);
        if ( HandObjInstance != nullptr) 
        {

                GetWorldTimerManager().SetTimer(AutoDestructTimer,this,&AGiantHand::DestroyHand,Duration,false,0.0f);

            
                FVector A = WeaponMesh->GetSocketLocation(EffectOriginSocketName);
                FVector B = EyeLocation + (EyeRotation.Vector() * MaxRange);

                ShotDirection = ( A != FVector(0) ) ? (WeaponMesh->GetSocketRotation(EffectOriginSocketName).Vector() + (GetOwner()->GetActorRightVector() * 30.0f) ) : EyeRotation.Vector();
                if ( GetLocalRole() == ROLE_Authority) 
                {
                    HitScanTrace.TraceFrom = A;
                }
                FPredictProjectilePathParams pparams;
                FPredictProjectilePathResult result;

                pparams.ActorsToIgnore.Add(GetOwner());
                pparams.ActorsToIgnore.Add(HandObjInstance);
                pparams.ActorsToIgnore.Add(this);
                pparams.bTraceWithChannel = true;
                pparams.bTraceWithCollision = true;
                pparams.DrawDebugType = EDrawDebugTrace::ForDuration;
                pparams.DrawDebugTime = Duration;
                pparams.ProjectileRadius = Radius;
                pparams.StartLocation = A;
                pparams.LaunchVelocity = ShotDirection * ProjectileLaunchSpeed; 
                pparams.SimFrequency = 10.0f;
                pparams.TraceChannel = ECC_GameTraceChannel1;
                pparams.MaxSimTime = Duration;

                if ( UGameplayStatics::PredictProjectilePath(GetWorld(),pparams,result) ) 
                {

                    UE_LOG(LogTemp,Warning,TEXT("Hit something"));

                                
                    LastTraceHit = result.HitResult.Location;
                    HitScanTrace.TraceTo = LastTraceHit;

                    

                }

                bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
                    this,
                    V,
                    WeaponMesh->GetSocketLocation(FName("Source")),
                    GetTraceHit(),
                    ProjectileLaunchSpeed,
                    false,
                    0,
                    0,
                    ESuggestProjVelocityTraceOption::DoNotTrace);

                if ( bHaveAimSolution )
                {
                    HandObjInstance->ExecuteAction(EGiantHandState::Default,V,GetTraceHit());


                } else {
                    HandObjInstance->ExecuteAction(EGiantHandState::Default,FVector(HandObjInstance->GetActorForwardVector() * ProjectileLaunchSpeed),FVector(ShotDirection * MaxRange));

                }

                if ( GetLocalRole() == ROLE_Authority) 
                {
                    HitScanTrace.TraceTo = GetTraceHit();

                }

        }        


        StopFireTime = GetWorld()->TimeSeconds;

    }


    void AGiantHand::DestroyHand()
    {
        GetWorldTimerManager().ClearTimer(AutoDestructTimer);

        if ( HandObjInstance != nullptr )
        {

            HandObjInstance->GetRootComponent()->SetVisibility(false);
            HandObjInstance->Destroy();
            HandObjInstance = nullptr;
        }

    }


    
    void AGiantHand::SpawnEffects(FVector TraceEnd)
    {

    }
    
    void AGiantHand::Tick(float DeltaTime) 
    {

        Super::Tick(DeltaTime);

 


    }   
    
    void AGiantHand::BeginPlay() 
    {
        Super::BeginPlay();
    }

    void AGiantHand::TraceTarget()
    {
        ServerTraceTarget();
    }

    void AGiantHand::ServerTraceTarget_Implementation() 
    {

        UE_LOG(LogTemp,Warning,TEXT("TraceTarget"));

		/* LINE TRACE: CALCULATE START & STOP POSITIONS */
		FVector EyeLocation; FRotator EyeRotation;
		
		GetOwner()->GetActorEyesViewPoint(EyeLocation,EyeRotation);

		FVector A = WeaponMesh->GetSocketLocation(EffectOriginSocketName);
		FVector B = EyeLocation + (EyeRotation.Vector() * 100);

		FVector ShotDirection = ( A != FVector(0) ) ? WeaponMesh->GetSocketRotation(EffectOriginSocketName).Vector() : EyeRotation.Vector();

		FVector TracerEndPoint = B;
        LastTraceHit = B;

		/* LINE TRACE: CONFIGURE PARAMETERS */
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetOwner());
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;
        DrawDebugLine(GetWorld(),A,B,FColor::White,false,1.0f,0,1.0f);	

		/* PERFORM LINE TRACE */
		FHitResult Hit;
		if ( GetWorld()->LineTraceSingleByChannel(Hit,A,B,ECC_GameTraceChannel1,QueryParams) ) 
		{

				/* ON HIT */
				//AActor* HitActor = Hit.GetActor();
				if ( GetLocalRole() == ROLE_Authority) 
				{
					HitScanTrace.TraceFrom = A;
                    HitScanTrace.TraceTo = Hit.Location;
                    
				}
				LastTraceHitTimeStamp = GetWorld()->TimeSeconds;
                LastTraceHit = Hit.Location;


        }




    }

    FVector AGiantHand::GetTraceHit() 
    {
        return LastTraceHit;
    }
