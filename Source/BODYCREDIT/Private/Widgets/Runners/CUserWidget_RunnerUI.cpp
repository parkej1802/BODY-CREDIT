#include "Widgets/Runners/CUserWidget_RunnerUI.h"
#include "Global.h"
#include "Components/CNoxHPComponent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UCUserWidget_RunnerUI::NativeConstruct()
{
	Super::NativeConstruct();

}

void UCUserWidget_RunnerUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UCNoxHPComponent* HP = CHelpers::GetComponent<UCNoxHPComponent>(GetOwningPlayerPawn());
	CheckNull(HP);

	// 체력 목표값
	const float TargetHealth = HP->Health;

	// 보간
	DisplayedHealthValue = FMath::FInterpTo(DisplayedHealthValue, TargetHealth, InDeltaTime, HealthTextLerpSpeed);

	// 텍스트로 표시
	if (Text_CurrentHealth)
	{
		int32 RoundedHealth = FMath::RoundToInt(DisplayedHealthValue);
		Text_CurrentHealth->SetText(FText::AsNumber(RoundedHealth));
	}

	// 목표값 계산
	const float TargetHealthPercent = HP->Health / HP->MaxHealth;

	// 현재 표시 값과 목표값을 보간
	DisplayedHealthPercent = FMath::FInterpTo(DisplayedHealthPercent, TargetHealthPercent, InDeltaTime, HealthLerpSpeed);

	// 실제 UI에 적용
	if (ProgressBar_Health)
		ProgressBar_Health->SetPercent(DisplayedHealthPercent);

	// 목표값 계산
	const float TargetStaminaPercent = HP->Stamina / HP->MaxStamina;

	// 현재 표시 값과 목표값을 보간
	DisplayedStaminaPercent = FMath::FInterpTo(DisplayedStaminaPercent, TargetStaminaPercent, InDeltaTime, StaminaLerpSpeed);

	// 실제 UI에 적용
	if (ProgressBar_Stamina)
		ProgressBar_Stamina->SetPercent(DisplayedStaminaPercent);

}

void UCUserWidget_RunnerUI::BindToHealthComponent(UCNoxHPComponent* InHealthComponent)
{
	CheckNull(InHealthComponent);

	DisplayedHealthValue = InHealthComponent->Health;

	// 델리게이트 바인딩
	InHealthComponent->OnHealthChanged.AddDynamic(this, &UCUserWidget_RunnerUI::OnHealthChanged);
	InHealthComponent->OnStaminaChanged.AddDynamic(this, &UCUserWidget_RunnerUI::OnStaminaChanged);

	// 초기값 세팅
	OnHealthChanged(InHealthComponent->GetCurrentHealth(), InHealthComponent->GetMaxHealth());

	// 초기값 세팅
	OnStaminaChanged(InHealthComponent->GetCurrentStamina(), InHealthComponent->GetMaxStamina());

}

void UCUserWidget_RunnerUI::UpdateCurrentDay(int32 InCurrentDay)
{

}

void UCUserWidget_RunnerUI::OnHealthChanged(float InCurrentHealth, float InMaxHealth)
{
	if (ProgressBar_Health)
		ProgressBar_Health->SetPercent(InCurrentHealth / InMaxHealth);

	if (Text_CurrentHealth)
		Text_CurrentHealth->SetText(FText::AsNumber(FMath::RoundToInt(InCurrentHealth)));

	if (Text_MaxHealth)
		Text_MaxHealth->SetText(FText::AsNumber(FMath::RoundToInt(InMaxHealth)));

}

void UCUserWidget_RunnerUI::OnStaminaChanged(float InCurrentStamina, float InMaxStamina)
{
	if (ProgressBar_Stamina)
		ProgressBar_Stamina->SetPercent(InCurrentStamina / InMaxStamina);

}
