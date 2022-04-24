// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Components/SphereComponent.h"
//#include "GameFramework/Actor.h"
//#include "IScorable.h"
#include "BaseProjectile.h"
#include "Projectile.generated.h"

UCLASS()
class LESSON_3_API AProjectile : public ABaseProjectile
{
	GENERATED_BODY()

		/*DECLARE_EVENT(AProjectile, FKillEvent);
		DECLARE_EVENT_OneParam(AProjectile, FExpEvent, FExpData);*/

public:	
	// Sets default values for this actor's properties
	AProjectile();

	// забираем некоторые компоненты и переносим в базовый класс проджектайла

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UParticleSystemComponent* ShootEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UAudioComponent* AudioEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USphereComponent* BlastRegion;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
		float MoveSpeed = 500;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
		float MoveRate = 0.005;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Projectile")
		int SelfDestructionSec = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Projectile")
		bool MortalFire = false;

	/*FKillEvent OnKillEvent;
	FExpEvent OnExpEventProjectile;*/

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	FTimerHandle Timer;
	FTimerHandle TimerDestruction;
	
	UFUNCTION() 
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void MoveTick();
	
	void SelfDestruction();
};

