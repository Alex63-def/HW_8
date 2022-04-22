// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UIHUD.generated.h"

/**
 * 
 */
// этот менеджер виджетов расчитан только на то что у нас на экране один виджет  

UENUM()
enum class EWidgetID : uint8
{
	Wid_MainMenu	= 0x0,
	Wid_ExitGame	= 0x1,
};

UCLASS()
class LESSON_3_API AUIHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	AUIHUD();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	// работает с энамом - прячет виджет - ищет виджет и передает в CreateWidgetByClass
	UFUNCTION(BlueprintCallable)
		UUserWidget* ShowWidget(const EWidgetID WidgetId, const int32 ZOrder = 0);

	// убирает виджет из вьюпорта
	UFUNCTION(BlueprintCallable)
		void HideWidget();

	UFUNCTION(BlueprintPure)
		UUserWidget* GetCurrentWidget() const { return CurrentWidget; }

protected:

	// создает виджет и выводит его на экран - работает уже склассом виджета и не взаимодействует с энамом
	UUserWidget* CreateWidgetByClass(const TSubclassOf<UUserWidget> WidgetClass, const int32 ZOrder = 0);

	// глючило запись - в общем как понял тут хрянтся для эелементов эеама настройки виджета - карта где он спавнится и класс виджета
	UPROPERTY(EditAnywhere)
		TMap<EWidgetID, TSubclassOf<UUserWidget>> WidgetClasses;

	// тут храним виджет который сейчас на экране 
	UPROPERTY()
		UUserWidget* CurrentWidget;

	// если хотим модернезировать что бы на экране было много виджетов
	/*
	UPROPERTY()
		TMap<EWidgetID, UUserWidget*> CurrentWidgets;
	*/
};
