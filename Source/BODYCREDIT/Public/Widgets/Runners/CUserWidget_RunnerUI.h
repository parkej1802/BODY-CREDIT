#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_RunnerUI.generated.h"

UCLASS()
class BODYCREDIT_API UCUserWidget_RunnerUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_Day;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* ProgressBar_Stamina;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* ProgressBar_Health;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_CurrentHealth;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_MaxHealth;

public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void BindToHealthComponent(class UCNoxHPComponent* InHealthComponent);

	UFUNCTION()
	void UpdateCurrentDay(int32 InCurrentDay);

	UFUNCTION()
	void OnHealthChanged(float InCurrentHealth, float InMaxHealth);

	UFUNCTION()
	void OnStaminaChanged(float InCurrentStamina, float InMaxStamina);

private:
	class UGameInstance* GameInstance;

private:
	float DisplayedHealthPercent = 1.0f;
	float HealthLerpSpeed = 5.0f;

	float DisplayedHealthValue = 0.0f;
	float HealthTextLerpSpeed = 5.0f;

	float DisplayedStaminaPercent = 1.0f;
	float StaminaLerpSpeed = 5.0f;

};
