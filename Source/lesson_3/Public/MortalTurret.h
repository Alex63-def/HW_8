// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Turret.h"
#include "MortalProjectile.h"
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

protected:

	virtual void BeginPlay() override;

private:

	virtual void Targeting() override;

	float ProjectileVelocity = 750.0f / 100;
	float ProjectileVelocityPow2 = FMath::Pow(ProjectileVelocity, 2);
	float ProjectileVelocityPow4 = FMath::Pow(ProjectileVelocity, 4);
	float G = 9.81f;
	float TurretZ{};
	float TargetZ{};
	FVector TurretPosition{};
};