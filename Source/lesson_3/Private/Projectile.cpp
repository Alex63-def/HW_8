// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "DamageTarget.h"
#include "UnitPawn.h"
#include "lesson_3/lesson_3.h"
#include "BaseFactory.h"
#include "DrawDebugHelpers.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AudioEffect = CreateDefaultSubobject<UAudioComponent>("AudioEffect");
	AudioEffect->SetupAttachment(Mesh);

	ShootEffect = CreateDefaultSubobject<UParticleSystemComponent>("ShootEffect");
	ShootEffect->SetupAttachment(Mesh);

	BlastRegion = CreateDefaultSubobject<USphereComponent>("BlastRegion");
	BlastRegion->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (MortalFire)
	{
		Collision->BodyInstance.bNotifyRigidBodyCollision = true;
		//Mesh->BodyInstance.bNotifyRigidBodyCollision = true;
		Collision->SetCollisionObjectType(ECC_MAX);
	}
	else
		Collision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);
	
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AProjectile::MoveTick, MoveRate, true); 

	GetWorld()->GetTimerManager().SetTimer(TimerDestruction, this, &AProjectile::SelfDestruction, SelfDestructionSec, false);
}
													
void AProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this)
		return;

	//GEngine->AddOnScreenDebugMessage(111111, 3, FColor::Red, FString::Printf(TEXT("OnBeginOverlap")), false);

	GetWorld()->GetTimerManager().ClearTimer(Timer);

	// сделаем что бы уничтожение было только при достижении хп = 0
	auto Target = Cast<IDamageTarget>(OtherActor);
	if (Target) // если может получить урон
	{
		FDamageData DamageData;
		DamageData.DamageValue = Damage;
		DamageData.Instigator = this;
		Target->TakeDamage(DamageData);
	}

	if (auto Unit = Cast<AUnitPawn>(OtherActor))
	{
		if (Unit->bDeath)
		{
			Unit->bDeath = false;

			FExpData ExpData;
			ExpData.ExperienceValue = Unit->Experience;
			ExpData.Enemy = OtherActor;

			if (OnExpEventProjectile.IsBound())
				OnExpEventProjectile.Broadcast(ExpData);

			if (OnKillEvent.IsBound())
				OnKillEvent.Broadcast();
		}
	}
	else if (auto Unitt = Cast<ABaseFactory>(OtherActor))
	{
		if (Unitt->bDeath)
		{
			Unitt->bDeath = false;

			FExpData ExpData;
			ExpData.ExperienceValue = Unitt->Experience;
			ExpData.Enemy = OtherActor;

			if (OnExpEventProjectile.IsBound())
				OnExpEventProjectile.Broadcast(ExpData);

			if (OnKillEvent.IsBound())
				OnKillEvent.Broadcast();
		}
	}

	// это роняет стены - передача физического усилия
	const auto Root = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
	if (Root && Root->IsSimulatingPhysics())
	{
		Root->AddImpulseAtLocation(GetActorForwardVector() * MoveSpeed/5, SweepResult.Location);
		//Root->AddForce(GetActorForwardVector() * 1000, Root->GetFName(), true);
		//GEngine->AddOnScreenDebugMessage(222222, 3, FColor::Red, FString::Printf(TEXT("OnBeginOverlap -- Impulse")), false);
	}

	auto Temp = GetActorLocation();
	Temp = GetActorLocation();
	SetActorLocation({-100, -100, -100});
	ShootEffect->SetWorldLocation(Temp);
	AudioEffect->SetWorldLocation(Temp);
	AudioEffect->Play();
	ShootEffect->ActivateSystem();
	////Destroy(); 
}

void AProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	//GEngine->AddOnScreenDebugMessage(333333, 3, FColor::Red, FString::Printf(TEXT("NotifyHit")), false);

	// при соприкосновении с поверхностью будет делать урон по площади
	TArray<FHitResult> Targets; // это наши цели									на сколько я понял тут находим всех с кем столкнулся радиус урона
	GetWorld()->SweepMultiByChannel(Targets, GetActorLocation(), GetActorLocation(), GetActorRotation().Quaternion(), ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(BlastRegion->GetScaledSphereRadius()));

	TSet<TWeakObjectPtr<AActor>> STargets;
	for (auto& el : Targets)
	{
		STargets.Add(el.Actor);
	}


	DrawDebugSphere(GetWorld(), GetActorLocation(), BlastRegion->GetScaledSphereRadius(), 16, FColor::Red, true);

	for (auto& HitTarget : STargets)
	{
		// сделаем что бы уничтожение было только при достижении хп = 0
		auto Target = Cast<IDamageTarget>(HitTarget/*.Actor*/);
		if (Target) // если может получить урон
		{
			FDamageData DamageData;
			DamageData.DamageValue = Damage;
			DamageData.Instigator = this;
			Target->TakeDamage(DamageData);
		}

		if (auto Unit = Cast<AUnitPawn>(HitTarget/*.Actor*/))
		{
			if (Unit->bDeath)
			{
				Unit->bDeath = false;

				FExpData ExpData;
				ExpData.ExperienceValue = Unit->Experience;
				ExpData.Enemy = Cast<AActor>(HitTarget/*.Actor*/);

				if (OnExpEventProjectile.IsBound())
					OnExpEventProjectile.Broadcast(ExpData);

				if (OnKillEvent.IsBound())
					OnKillEvent.Broadcast();
			}
		}
		else if (auto Unitt = Cast<ABaseFactory>(HitTarget/*.Actor*/))
		{
			if (Unitt->bDeath)
			{
				Unitt->bDeath = false;

				FExpData ExpData;
				ExpData.ExperienceValue = Unitt->Experience;
				ExpData.Enemy = Cast<AActor>(HitTarget/*.Actor*/);

				if (OnExpEventProjectile.IsBound())
					OnExpEventProjectile.Broadcast(ExpData);

				if (OnKillEvent.IsBound())
					OnKillEvent.Broadcast();
			}
		}

		if (Cast<ABaseProjectile>(HitTarget/*.Actor*/))
		{
			continue;
		}

		if (HitTarget.IsValid())
		{
			const auto Root = Cast<UPrimitiveComponent>(HitTarget->GetRootComponent());
			if (Root && Root->IsSimulatingPhysics())
			{
				auto Impulse = (HitTarget->GetActorLocation() - GetActorLocation()) * 10;
				Root->AddImpulseAtLocation(Impulse, HitTarget->GetActorLocation());
				//Root->AddForce(GetActorForwardVector() * 1000, Root->GetFName(), false);
				//GEngine->AddOnScreenDebugMessage(444444, 3, FColor::Red, FString::Printf(TEXT("NotifyHit -- Impulse")), false);
			}
		}
	}
	auto Temp = GetActorLocation();
	SetActorLocation({ -100, -100, -100 });
	ShootEffect->SetWorldLocation(Temp);
	AudioEffect->SetWorldLocation(Temp);
	AudioEffect->Play();
	ShootEffect->ActivateSystem();
}

void AProjectile::MoveTick()
{
	const auto Location = GetActorLocation();
	
	SetActorLocation(Location + GetActorForwardVector() * MoveRate * MoveSpeed, true); 
	
	/*auto s = GetVelocity();
	UE_LOG(LogTanks, Warning, TEXT("%f"), s);*/
}

void AProjectile::SelfDestruction()
{
	/*AudioEffect->Play();
	ShootEffect->ActivateSystem();*/
	Destroy();
}

