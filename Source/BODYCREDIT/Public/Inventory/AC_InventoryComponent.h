// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/AC_InventoryBaseComponent.h"
#include "AC_InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BODYCREDIT_API UAC_InventoryComponent : public UAC_InventoryBaseComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_InventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACNox_Runner* PlayerCharacter;

	UPROPERTY()
	class ACNox_Controller* pc;

	void SetupInputBinding(class UEnhancedInputComponent* Input);

// Widget
public:
	UPROPERTY(EditAnywhere, Category = Widget)
	TSubclassOf<UUserWidget> InventoryWidget;

	UPROPERTY()
	class UInventory_Widget* InventoryMainUI;

	UPROPERTY(EditAnywhere, Category = Widget)
	TSubclassOf<UUserWidget> PauseGameWidget;

	UPROPERTY()
	class ULobbyWidget_Pause* PauseGameUI;

// Input
public:
	bool bIsInventoryMode = false;
	bool bIsLootableMode = false;
	bool bIsPauseMode = false;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_InventoryMode;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_RotateItem;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_LootableItem;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_Pause;
	
	void ShowInventory();
	void RotateItem();
	void ShowLootableInventory();
	void PauseGame();
};
