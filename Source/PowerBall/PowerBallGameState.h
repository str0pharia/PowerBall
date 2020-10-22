// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "PowerBallGameState.generated.h"

/**
 * 
 */


class ABasketBall;

UCLASS()
class POWERBALL_API APowerBallGameState : public AGameState
{
	GENERATED_BODY()


	public:

		float GetServerWorldTimeSeconds() const override;

		ABasketBall* GetBasketBall();

		UFUNCTION(BlueprintCallable, Category = "PowerBall")
		void SetBasketBall(ABasketBall* Ball);

	protected:

		ABasketBall* _Ball = nullptr;


	
};
