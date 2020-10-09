// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"
#include "GameFramework/GameStateBase.h"

void APlayerCharacterController::ServerRequestServerTime_Implementation(APlayerController* requester, float requestWorldTime
)
{
    float serverTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
    ClientReportServerTime(requestWorldTime, serverTime);
}

bool APlayerCharacterController::ServerRequestServerTime_Validate(APlayerController* requester,float requestWorldTime
)
{
    return true;
}

void APlayerCharacterController::ClientReportServerTime_Implementation(float requestWorldTime,float serverTime
)
{
    // Apply the round-trip request time to the server's         
    // reported time to get the up-to-date server time
    float roundTripTime = GetWorld()->GetTimeSeconds() - requestWorldTime;
    float adjustedTime = serverTime + (roundTripTime * 0.5f);
    ServerTime = adjustedTime;
}

void APlayerCharacterController::ReceivedPlayer()
{
    Super::ReceivedPlayer();

    if(IsLocalController())
    {
        ServerRequestServerTime(
            this,
            GetWorld()->GetTimeSeconds()
        );
    }
}