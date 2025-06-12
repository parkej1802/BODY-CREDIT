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

	UFUNCTION()
	void OnContinueHovered();

	UFUNCTION()
	void OnContinueUnhovered();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Head;

	UFUNCTION()
	void OnHeadClicked();

	UFUNCTION()
	void OnHeadHovered();

	UFUNCTION()
	void OnHeadUnhovered();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Body;
	
	UFUNCTION()
	void OnBodyClicked();

	UFUNCTION()
	void OnBodyHovered();

	UFUNCTION()
	void OnBodyUnhovered();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Arm;
	
	UFUNCTION()
	void OnArmClicked();

	UFUNCTION()
	void OnArmHovered();

	UFUNCTION()
	void OnArmUnhovered();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Leg;
	
	UFUNCTION()
	void OnLegClicked();

	UFUNCTION()
	void OnLegHovered();

	UFUNCTION()
	void OnLegUnhovered();

	UPROPERTY()
	class UNetGameInstance* GI;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_Continue_Hovered;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_Head_Hovered;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_Body_Hovered;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_Arm_Hovered;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_Leg_Hovered;

	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* ImageHead;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* ImageBody;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* ImageArm;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* ImageLeg;

	class UImage* PreviousImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACMainGM* GameMode;

	void TurnOffPreviousImage();

	void TurnOnPreviousImage();

};
