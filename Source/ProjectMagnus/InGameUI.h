// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonPressed);

/**
 * 
 */
UCLASS()
class PROJECTMAGNUS_API UInGameUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:

	FOnButtonPressed onButtonPressed;

	UFUNCTION(BlueprintImplementableEvent, Category = "LevelUpSequence")
	void LevelUpEvent(float oldHValue, float newHValue, float oldSValue, float newSValue, float oldStrValue, float newStrValue, float oldEValue, float newEValue, float oldAValue, float newAValue, float oldDValue, float newDValue, float oldAPValue, float newAPValue, float oldLevelValue, float newLevelValue);
	UFUNCTION(BlueprintCallable, Category = "LevelUpSequence")
	void ShowVigorText(float oldValue, float newValue);
	UFUNCTION(BlueprintCallable, Category = "LevelUpSequence")
	void ShowLevelUpText(float oldValue, float newValue, class UTextBlock* oldText, UTextBlock* newText);


	UFUNCTION()
	void GetNewWeaponInfo(class AWeapon* weapon);
	UFUNCTION()
	void ShowLevelUpScreen(class APlayerCharacter* unit, float oldV, float oldS, float oldStr, float oldE, float oldA, float oldD);

	UFUNCTION()
	void DisplayTargetStats(class ACharacter_Base* target, float health, float maxHealth);
	UFUNCTION()
	void DisplayEnemyTargetStats(class ABaseEnemy* target, float health, float maxHealth);

	UFUNCTION()
	void DisplayTargetInfo(bool displayOrNot);

	UFUNCTION()
	void UpdateHealth(float health, float maxHealth);

	UFUNCTION()
	void UpdateStamina(float health, float maxHealth);

	UFUNCTION()
	void UpdateExperience(float exp, float mExp);

	UFUNCTION()
	void UpdateAmmoCount(int ammo, int ammoReserves);

	UFUNCTION()
	void NewUnitGiven(APlayerCharacter* unit);

	UFUNCTION()
	void SetValueSegments(float maxSegments);

	UFUNCTION()
	void SetAPText(float newValue);

	UFUNCTION()
	void UpdateRanges(float maxHealth, float maxStam, float exp, float mExp);

	UFUNCTION()
	void SetForecast(int STK, int ammoLeft);

	UFUNCTION()
	void ClearForecast();

	UFUNCTION()
	void UnhideButton();

	UFUNCTION()
	void StopFiring();

	UFUNCTION()
	void SetTipText(FString text);

	void EnableFieldCanvas();
	void HideLevelUpTextPair(UTextBlock* one, UTextBlock* two);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* oldHealthLevelUpText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* newHealthLevelUpText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* oldStaminaText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* newStaminaText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* oldStrengthText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* newStrengthText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* oldEnduranceText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* newEnduranceText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* oldAgilityText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* newAgilityText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* oldDexText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* newDexText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* oldAPText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* newAPText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* oldLevelText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* newLevelText;


private:

	UFUNCTION()
	void PossessNewUnit(APlayerCharacter* newUnit);



	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* UISwitcher;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* tacticsCanvas;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* inGameCanvas;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* levelUpCanvas;

	UPROPERTY(meta = (BindWidget))
	class UImage* levelUpPanelImage;

	UPROPERTY(meta = (BindWidget))
	class UImage* unitImageLevelUp;





	UPROPERTY(meta = (BindWidget))
	UImage* crossHair;

	UPROPERTY(meta = (BindWidget))
	UImage* firearmForecastImage;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* targetHealthBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* targetCurrentHealthText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* targetMaxHealthText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* targetName;

	UPROPERTY(meta = (BindWidget))
	class UButton* stopFiringOverlayButton;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* targetOverlay;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* tipText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* shotsToKillText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ammoLeftText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ammoCount;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ammoReservesText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* APCount;
	UPROPERTY(meta = (BindWidget))
	UImage* weaponIcon;
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* forecastHoriBox;


	//UPROPERTY(meta = (BindWIdget))
	//class UProgressBar* healthBar;

	//UPROPERTY(meta = (BindWIdget))
	//class UProgressBar* staminaBar;

	UPROPERTY(meta = (BindWidget))
	class UValueGauge* healthGauge;

	UPROPERTY(meta = (BindWidget))
	class UValueGauge* staminaGauge;

	UPROPERTY(meta = (BindWidget))
	class UValueGauge* experienceGauge;

	UPROPERTY(meta = (BindWidget))
	class UAPValue* APGauge;

	UPROPERTY(meta = (BindWidget))
	class UListView* unitList;

	
};
