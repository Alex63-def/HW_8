// Fill out your copyright notice in the Description page of Project Settings.


#include "UIHUD.h"
#include "Blueprint/UserWidget.h"

AUIHUD::AUIHUD()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AUIHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AUIHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UUserWidget* AUIHUD::ShowWidget(const EWidgetID WidgetId, const int32 ZOrder)
{
	HideWidget();

	TSubclassOf<UUserWidget>* ClassPtr = WidgetClasses.Find(WidgetId);
	if (ClassPtr && *ClassPtr)
	{
		CreateWidgetByClass(*ClassPtr, ZOrder);
	}

	return CurrentWidget;
}

void AUIHUD::HideWidget()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}
}

UUserWidget* AUIHUD::CreateWidgetByClass(const TSubclassOf<UUserWidget> WidgetClass, const int32 ZOrder)
{
	CurrentWidget = CreateWidget(GetOwningPlayerController(), WidgetClass);
	CurrentWidget->AddToViewport(ZOrder);
	return CurrentWidget;
}
