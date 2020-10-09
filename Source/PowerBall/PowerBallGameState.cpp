// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerBallGameState.h"
#include "PlayerCharacterController.h"

float APowerBallGameState::GetServerWorldTimeSeconds() const
{
    if(APlayerCharacterController* pc = (APlayerCharacterController*)GetGameInstance()->
        GetFirstLocalPlayerController(GetWorld())
    )
    {
        return pc->GetServerTime();
    }
    else
    {
        return GetWorld()->GetTimeSeconds();
    }
}



ABasketBall* APowerBallGameState::GetBasketBall()
{

    return _Ball;

}

void APowerBallGameState::SetBasketBall(ABasketBall* Ball)
{

    _Ball = Ball;

}

