// Fill out your copyright notice in the Description page of Project Settings.


#include "GiantHand.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "DrawDebugHelpers.h"
#include "Components/SplineComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Controller.h"
#include "PlayerCharacterController.h"
#include "PlayerCharacter.h"
#include "Hand.h"

    AGiantHand::AGiantHand() 
    {
        static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/Blueprints/Summon/Hand/HandSummon'"));
        if (ItemBlueprint.Object){
            HandTemplate = (UClass*)ItemBlueprint.Object->GeneratedClass;
        }


        PrimaryActorTick.bCanEverTick = false;

	    //
	    EffectOriginSocketName = FName("Source");

        Duration = 4.0f;

        MinRange = 100.0f;
        MaxRange = 350.0f;

 
        bNetLoadOnClient = true;
        SetReplicates(true);

    }

    void AGiantHand::Fire()
    {
        
    
        if ( GetLocalRole() < ROLE_Authority)
        {
            
            ServerFire();
        

        } 
        StartFireTime = GetWorld()->TimeSeconds;


    }

    
    void AGiantHand::ServerFire()
    {      

      
    }


    void AGiantHand::StopFire() 
    {   
   
/*
        if ( GetLocalRole() <= ROLE_Authority)
        {
            
            ServerStopFire();
        

        } 
*/


        SpawnHand();
/*

            FVector V;
            FActorSpawnParameters params;
            params.Owner = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
            params.Instigator = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
            FVector EyeLocation; FRotator EyeRotation;
            GetOwner()->GetActorEyesViewPoint(EyeLocation,EyeRotation);

            FVector SpawnPoint = EyeLocation + (GetOwner()->GetActorForwardVector() * 20.0);
            FVector ShotDirection = EyeRotation.Vector() * 1.f;
            FRotator Rot = ShotDirection.Rotation();
        
            
            ProjectileInstance = GetWorld()->SpawnActor(HandTemplate,&SpawnPoint,&Rot,params);
*/        
            /*
            bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
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

                UE_LOG(LogTemp,Warning,TEXT("found aim solution"));
                 hand->ExecuteAction(EGiantHandState::Default,V,FVector(0),Duration);


            } else {
                UE_LOG(LogTemp,Warning,TEXT("could not find aim solution"));

                hand->ExecuteAction(EGiantHandState::Default,FVector(hand->GetActorForwardVector() * ProjectileLaunchSpeed),FVector(ShotDirection * MaxRange),Duration);

            }

            */


           // ((APlayerCharacter*)UGameplayStatics::GetPlayerPawn(GetWorld(),0))->LaunchHand(EyeLocation,EyeRotation,ShotDirection,GetOwner(),UGameplayStatics::GetPlayerPawn(GetWorld(),0));

     
     StopFireTime = GetWorld()->TimeSeconds;
     GetWorldTimerManager().ClearTimer(PrimaryActionTimer);
       
    }

    // Cast/Trigger Spell
    void AGiantHand::ServerStopFire() 
    {


       
           //Fire();

           // OnRep_ProjectileInstance();

 
     

    }
    
    void AGiantHand::SpawnEffects(FVector TraceEnd)
    {

    }

    void AGiantHand::SpawnHand_Implementation()
    {

            FVector V;
            FActorSpawnParameters params;
            params.Owner = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
            params.Instigator = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
            FVector EyeLocation; FRotator EyeRotation;
            GetOwner()->GetActorEyesViewPoint(EyeLocation,EyeRotation);

            FVector SpawnPoint = EyeLocation + (GetOwner()->GetActorForwardVector() * 20.0);
            FVector ShotDirection = EyeRotation.Vector() * 1.f;
            FRotator Rot = ShotDirection.Rotation();
        
            
            GetWorld()->SpawnActor(HandTemplate,&SpawnPoint,&Rot,params);
    }

    bool AGiantHand::SpawnHand_Validate()
    {
        return true;
    }



    void AGiantHand::OnRep_ProjectileInstance() 
    {
        Super::OnRep_ProjectileInstance();

        if ( ProjectileInstance != nullptr )
            ProjectileInstance->GetRootComponent()->SetVisibility(true);
        
    }
 
    