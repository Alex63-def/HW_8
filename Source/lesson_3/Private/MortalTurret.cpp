// Fill out your copyright notice in the Description page of Project Settings.


#include "MortalTurret.h"
#include "Kismet/KismetMathLibrary.h"
#include "lesson_3/lesson_3.h"
#include "TankPawn.h"

AMortalTurret::AMortalTurret() : ATurret()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	
}

void AMortalTurret::BeginPlay()
{
	Super::BeginPlay();

	TurretZ = SceneComponent->GetComponentLocation().Z /*/ 100*/;
	TurretPosition = GetActorLocation();
}

void AMortalTurret::Targeting()
{

	if (!Target.IsValid())
	{
		return;
	}
	
	auto TargetRotation = UKismetMathLibrary::FindLookAtRotation(SceneComponent->GetComponentLocation(), Target->GetActorLocation()); 
	
	if (!TargetZ)
	{
		auto TTarget = Cast<ATankPawn>(Target);
		TargetZ = TTarget->BodyMesh->GetComponentLocation().Z /*/ 150*/;
	}
	
	auto TargetPosition = Target->GetActorLocation();
	auto Distance = FVector::Dist2D(TurretPosition, TargetPosition) / 150;

	float P = FMath::RadiansToDegrees(FMath::Atan(1/FMath::Tan(ProjectileVelocityPow2 + FMath::Sqrt(ProjectileVelocityPow4 - G * (G * FMath::Pow(Distance, 2) + 2 * ProjectileVelocityPow2 * (0.5/*TurretZ - TargetZ*/))) / (G * Distance))));
	if (Distance > 4.5f)
	{
		P = 45.0f;
	}

	float RotateY = TargetRotation.Yaw;

	SceneComponent->SetWorldRotation(FMath::Lerp(SceneComponent->GetComponentRotation(), { P, RotateY, 0.f }, TargetingSpeed));

	FRotator t = { P, RotateY, 0.f };
	
	UE_LOG(LogTanks, Warning, TEXT("%s"), *t.ToString());

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

	PlayerDirection.Normalize(); 

	auto Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDirection, PlayerDirection))); 

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
