// Fill out your copyright notice in the Description page of Project Settings.
#include "Inventory/AC_InventoryComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "Inventory/Inventory_Widget.h"
#include "Characters/CNox_Runner.h"

// Sets default values for this component's properties
UAC_InventoryComponent::UAC_InventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UInputAction>TempIA_InventoryMode(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Inventory.IA_Inventory'"));
	if (TempIA_InventoryMode.Succeeded())
	{
		IA_InventoryMode = TempIA_InventoryMode.Object;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> TempWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Inventory/WBP_MainInventory.WBP_MainInventory'"));
	if (TempWidget.Succeeded())
	{
		InventoryWidget = TempWidget.Class;
	}
}

// Called when the game starts
void UAC_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();

	if ((pc = OwnerActor->GetInstigatorController<APlayerController>()) != nullptr)
	{
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(pc->InputComponent);
		if (EnhancedInputComponent)
		{
			SetupInputBinding(EnhancedInputComponent);
		}
	}

	if (OwnerActor)
	{
		PlayerCharacter = Cast<ACNox_Runner>(OwnerActor);
	}
}

// Called every frame
void UAC_InventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAC_InventoryComponent::SetupInputBinding(class UEnhancedInputComponent* Input)
{
	Input->BindAction(IA_InventoryMode, ETriggerEvent::Started, this, &UAC_InventoryComponent::ShowInventory);
}

void UAC_InventoryComponent::ShowInventory()
{
	if (!bIsInventoryMode) {
		bIsInventoryMode = true;
		if (InventoryWidget)
		{
			InventoryMainUI = CreateWidget<UInventory_Widget>(GetWorld(), InventoryWidget);
		}
		if (InventoryMainUI)
		{
			InventoryMainUI->AddToViewport();
		}
		FInputModeGameAndUI UIInputMode;
		pc->SetInputMode(UIInputMode);
		pc->bShowMouseCursor = true;
	}
	else {
		bIsInventoryMode = false;
		if (InventoryMainUI)
		{
			InventoryMainUI->RemoveFromParent();
		}
		FInputModeGameOnly GameInputMode;
		pc->SetInputMode(GameInputMode);
		pc->bShowMouseCursor = false;
	}
}

