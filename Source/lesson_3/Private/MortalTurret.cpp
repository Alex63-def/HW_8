// Fill out your copyright notice in the Description page of Project Settings.


#include "MortalTurret.h"
#include "Kismet/KismetMathLibrary.h"

AMortalTurret::AMortalTurret() : ATurret()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	
}

void AMortalTurret::Targeting()
{

	if (!Target.IsValid())
	{
		return;
	}

	auto TargetRotation = UKismetMathLibrary::FindLookAtRotation(SceneComponent->GetComponentLocation(), Target->GetActorLocation()); // TurretMesh

	SceneComponent->SetWorldRotation(FMath::Lerp(SceneComponent->GetComponentRotation(), TargetRotation, TargetingSpeed));

	int TargetRotationY = TargetRotation.Yaw * 10;
	int TargetRotationR = TargetRotation.Roll * 10;
	int TargetRotationP = TargetRotation.Pitch * 10;

	int SceneComponentY = SceneComponent->GetComponentRotation().Yaw * 10;
	int SceneComponentR = SceneComponent->GetComponentRotation().Roll * 10;
	int SceneComponentP = SceneComponent->GetComponentRotation().Pitch * 10;

	if (TargetRotationY != SceneComponentY || TargetRotationR != SceneComponentR || TargetRotationP != SceneComponentP)
	{
		if (!AudioEffectRotateTurret->IsActive())
		{
			AudioEffectRotateTurret->Play();
			IsTurretRotation = true;
		}
	}

	if (TargetRotationY == SceneComponentY && TargetRotationR == SceneComponentR && TargetRotationP == SceneComponentP)
	{
		AudioEffectRotateTurret->Stop();

		if (!AudioEffectStopRotateTurret->IsActive() && IsTurretRotation)
		{
			AudioEffectStopRotateTurret->Play();
			IsTurretRotation = false;
		}
	}

	auto TargetingDirection = TurretMesh->GetForwardVector();

	auto PlayerDirection = Target->GetActorLocation() - GetActorLocation();

	PlayerDirection.Normalize(); // наверное это как число по модулю 

	auto Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDirection, PlayerDirection))); // это операция умножения одного вектора на другой и по теореме косинусов вроде найдет угол между векторами

	FHitResult Result;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(Cannon);
	Params.bTraceComplex = true;
	auto Start = CannonPosition->GetComponentLocation();
	auto End = CannonPosition->GetForwardVector() * 650 + Start;
	bool bHasHit = GetWorld()->LineTraceSingleByChannel(Result, TurretMesh->GetComponentLocation(), Target->GetActorLocation() + 10, ECollisionChannel::ECC_Visibility, Params);

	auto Tank = OtherActors[0]->GetActorLocation();

	if (bHasHit)
	{
		End = Result.Location;
	}
	else
		End = Tank;

	if (End == Tank)
	{
		if (Angle < AimSlack)
		{
			Fire();
		}
	}
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.05, 0, 1);
}