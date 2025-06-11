#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CLobbyWidget_Play.generated.h"

UCLASS()
class BODYCREDIT_API UCLobbyWidget_Play : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Continue;

	UFUNCTION()
	void OnContinueClicked();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Head;

	UFUNCTION()
	void OnHeadClicked();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Body;
	
	UFUNCTION()
	void OnBodyClicked();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Arm;
	
	UFUNCTION()
	void OnArmClicked();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Leg;
	
	UFUNCTION()
	void OnLegClicked();

	UPROPERTY()
	class UNetGameInstance* GI;

	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* ImageHead;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* ImageBody;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* ImageArm;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* ImageLeg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACMainGM* GameMode;

};
