// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */

class UButton;

UCLASS(Abstract)
class LESSON_3_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// подписки на делегаты
	virtual void NativeConstruct() override;

	// отписки от делегатов при удалении виджета
	virtual void NativeDestruct() override;

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
		UButton* NewGameBtn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton* QuitBtn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* NewGameAnimation;

	UFUNCTION()
		void OnNewGameClicked();

	UFUNCTION()
		void OnQuitClicked();
};
