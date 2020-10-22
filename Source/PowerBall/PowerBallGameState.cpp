// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerBallGameState.h"
#include "PlayerCharacterController.h"


float APowerBallGameState::GetServerWorldTimeSeconds() const
{
    APlayerCharacterController* pc = GetWorld()->GetFirstPlayerController<APlayerCharacterController>();
    

    if ( pc != nullptr )
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

