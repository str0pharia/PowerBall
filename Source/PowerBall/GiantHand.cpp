// Fill out your copyright notice in the Description page of Project Settings.


#include "GiantHand.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"
#include "Hand.h"

    AGiantHand::AGiantHand() 
    {

        PrimaryActorTick.bCanEverTick = true;

	    EffectOriginSocketName = FName("Source");

	    SetReplicates(true);

        static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/Blueprints/Summon/Hand/HandSummon'"));
        if (ItemBlueprint.Object){
            HandTemplate = (UClass*)ItemBlueprint.Object->GeneratedClass;
        }
    }

    void AGiantHand::Fire()
    {
        ServerFire();
    }

    void AGiantHand::ServerFire()
    {
        
        Recording = true;



        GetWorldTimerManager().SetTimer(RecordPathTimer, this, &AGiantHand::AddSplinePoint, 1.0f, true, HitIntervalSeconds);


    }

    void AGiantHand::AddSplinePoint() 
    {
        float LocationX;
        float LocationY;

        if ( UGameplayStatics::GetPlayerController(GetWorld(),0)->GetMousePosition(LocationX,LocationY) )
        {   
            FVector Location;
            FVector Direction;

            if ( UGameplayStatics::GetPlayerController(GetWorld(),0)->DeprojectScreenPositionToWorld(LocationX,LocationY,Location, Direction) ) 
            {
              //  SplineComponent->AddSplinePoint(Location, ESplineCoordinateSpace::World, true);
                
                FSplinePoint p;
                p.Position = Location;
                p.Rotation = Direction.Rotation();
                SplinePoints.Add(p);

            }


        } 
   
        if ( MaxTicks >= Duration )
        {
            Recording = false;
            GetWorldTimerManager().ClearTimer(RecordPathTimer);

            if (HandTemplate != nullptr && HandObjInstance == nullptr)
            {

                FActorSpawnParameters params;

                params.Name = "Hand";
                params.Owner = GetOwner();
                params.Instigator = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
                const FVector loc = GetOwner()->GetActorLocation();
                const FRotator rot = GetOwner()->GetActorRotation();
                HandObjInstance = GetWorld()->SpawnActor<AHand>(HandTemplate,loc,rot,params);
        
                if ( HandObjInstance != nullptr)
                {
                    FSplinePoint p;
                    p.Position = HandObjInstance->GetActorLocation();
                    p.Rotation = GetOwner()->GetActorForwardVector().Rotation();
                    
                    SetOrigin(p);
                    HandObjInstance->GetSplineComponent()->AddPoints(SplinePoints);
                }
            }
            

        }

    }

    void AGiantHand::SetOrigin(FSplinePoint SplinePoint)
    {

      //  SplineComponent->AddSplinePoint(Location,)

        SplinePoints.Add(SplinePoint);
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