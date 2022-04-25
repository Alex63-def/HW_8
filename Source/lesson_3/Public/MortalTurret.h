// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Turret.h"
#include "MortalTurret.generated.h"

/**
 * 
 */
UCLASS()
class LESSON_3_API AMortalTurret : public ATurret
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AMortalTurret();

private:

	virtual void Targeting() override;
};
