// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CGuideWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCGuideWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;
	
	void SetButtonColor(UButton* SelectedBtn);
	void SetGuideArray(const TArray<UTexture2D*>& InArray, UButton* SelectedBtn);
	void UpdateGuide();

private:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UButton* Btn_GameDescription;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UButton* Btn_Key;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UButton* Btn_Player;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UButton* Btn_Inventory;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UButton* Btn_Enemy;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UButton* Btn_Map;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UButton* Btn_Prev;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UButton* Btn_Later;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UTextBlock* Txt_Page;
	
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UImage* Img_Guide;

	//=============================
	int32 CurPage = 0;
	int32 MaxPage = 0;
	int32 GuideIndex = 0;
	const TArray<UTexture2D*>* CurrentArray = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category=Image)
	TArray<UTexture2D*> GameDescArray;
	UPROPERTY(EditDefaultsOnly, Category=Image)
	TArray<UTexture2D*> KeyGuideArray;
	UPROPERTY(EditDefaultsOnly, Category=Image)
	TArray<UTexture2D*> PlayerGuideArray;
	UPROPERTY(EditDefaultsOnly, Category=Image)
	TArray<UTexture2D*> InventoryGuideArray;
	UPROPERTY(EditDefaultsOnly, Category=Image)
	TArray<UTexture2D*> EnemyGuideArray;
	UPROPERTY(EditDefaultsOnly, Category=Image)
	TArray<UTexture2D*> MapGuideArray;

private:
	UFUNCTION()
	void OnGameDescClicked();
	UFUNCTION()
	void OnKeyClicked();
	UFUNCTION()
	void OnPlayerClicked();
	UFUNCTION()
	void OnInventoryClicked();
	UFUNCTION()
	void OnEnemyClicked();
	UFUNCTION()
	void OnMapClicked();
	UFUNCTION()
	void OnPrevClicked();
	UFUNCTION()
	void OnLaterClicked();
};
