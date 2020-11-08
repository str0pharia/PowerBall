// Fill out your copyright notice in the Description page of Project Settings.


#include "GiantHand.h"
#include "Kismet/GameplayStatics.h"
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


        static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/Blueprints/Summon/Hand/HandSummon'"));
        if (ItemBlueprint.Object){
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

        if ( HandObjInstance != nullptr ) 
        {
            HandObjInstance->Destroy();
            HandObjInstance = nullptr;           
            return;
        }

		if ( PrimaryActionTimer.IsValid() )
            return;

        GetWorldTimerManager().SetTimer(TraceTargetTimer,this,&AGiantHand::TraceTarget,0.5f,true,0.5f);
        GetWorldTimerManager().SetTimer(PrimaryActionTimer,this,&AGiantHand::AbortFire,0.5f,true,0.5f);


        

    }

    void AGiantHand::AbortFire()
    {

        ServerAbortFire();


    }

    void AGiantHand::ServerAbortFire() 
    {
        if ( TraceTargetTimer.IsValid() )
            GetWorldTimerManager().ClearTimer(TraceTargetTimer);

        if ( PrimaryActionTimer.IsValid() )
            GetWorldTimerManager().ClearTimer(PrimaryActionTimer);
             
    }


    void AGiantHand::StopFire() 
    {   
        ServerStopFire();
    }




    // Cast/Trigger Spell
    void AGiantHand::ServerStopFire() 
    {

        if ( PrimaryActionTimer.IsValid() )
            GetWorldTimerManager().ClearTimer(PrimaryActionTimer);

        if ( TraceTargetTimer.IsValid() )
            GetWorldTimerManager().ClearTimer(TraceTargetTimer);

        if ( HandObjInstance != nullptr)
            return;

        if ( LockedTarget == nullptr )
            return;

        
        
        FVector EyeLocation; FRotator EyeRotation;
                
        GetOwner()->GetActorEyesViewPoint(EyeLocation,EyeRotation);

        FVector ShotDirection = EyeRotation.Vector() * 100.0f;

        FActorSpawnParameters params;

        params.Owner = GetOwner();
        params.Instigator = UGameplayStatics::GetPlayerPawn(GetWorld(),0);

        //FVector dir = (SelectedPath->SplineEnd - SelectedPath->SplineStart);
        HandObjInstance = GetWorld()->SpawnActor<AHand>(HandTemplate,GetRootComponent()->GetComponentLocation(),GetRootComponent()->GetComponentRotation(),params);
        if ( HandObjInstance != nullptr && LockedTarget != nullptr) 
        {


            //LastNode = HandObjInstance->GetSplinePath()->AddPathNode(StartPos);

           // GetWorldTimerManager().SetTimer(PrimaryActionTimer, this, &AGiantHand::AbortFire, 10.0f, false, 10.0f);

         //   HandObjInstance->GetProjectileMovement()->HomingAccelerationMagnitude = true;
         //   HandObjInstance->GetProjectileMovement()->bRotationFollowsVelocity = true;
            HandObjInstance->GetProjectileMovement()->HomingTargetComponent = LockedTarget;


        } else {
            AbortFire();
            
        }

     
                /*
                FVector EyeLocation; FRotator EyeRotation;
                
                GetOwner()->GetActorEyesViewPoint(EyeLocation,EyeRotation);

                FVector ShotDirection = ( !LastNode.IsValid() ) ? WeaponMesh->GetSocketRotation(EffectOriginSocketName).Vector() : LastNode.Rotation.Vector();


                EndPos = LastNode.Position + (ShotDirection * 15.0f);
                HandObjInstance->SetActorRotation(ShotDirection.Rotation());

                HandObjInstance->Move(FVector::DistSquared(StartPos,EndPos));
                */

               // HandObjInstance->GetOwner()->Destroy();
                //HandObjInstance = nullptr;
    

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

    void AGiantHand::ServerTraceTarget_Implementation()
    {
        TraceTarget();
    }

    void AGiantHand::TraceTarget() 
    {

        if ( GetLocalRole() < ROLE_Authority )
        {

            ServerTraceTarget();

        }


		/* LINE TRACE: CALCULATE START & STOP POSITIONS */
		FVector EyeLocation; FRotator EyeRotation;
		
		GetOwner()->GetActorEyesViewPoint(EyeLocation,EyeRotation);

		FVector A = WeaponMesh->GetSocketLocation(EffectOriginSocketName);
		FVector B = EyeLocation + (EyeRotation.Vector() * 10000);

		FVector ShotDirection = ( A != FVector(0) ) ? WeaponMesh->GetSocketRotation(EffectOriginSocketName).Vector() : EyeRotation.Vector();

		FVector TracerEndPoint = B;

		/* LINE TRACE: CONFIGURE PARAMETERS */
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetOwner());
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		/* PERFORM LINE TRACE */
		FHitResult Hit;
		if ( GetWorld()->LineTraceSingleByChannel(Hit,A,B,ECC_GameTraceChannel1,QueryParams) ) 
		{

				/* ON HIT */
				AActor* HitActor = Hit.GetActor();


                if ( HitActor != nullptr ) 
                {
                    LockedTarget = HitActor->GetRootComponent();

                    if (LockedTarget->IsValid() )
                    {
                        StopFire();
                    }
                }



        }

    }