// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Projectiles.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UProjectiles : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class POWERBALL_API IProjectiles
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:


	void SetVelocity(FVector Velocity);

	
};
