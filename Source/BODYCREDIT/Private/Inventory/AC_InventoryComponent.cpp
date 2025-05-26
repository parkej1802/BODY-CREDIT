// Fill out your copyright notice in the Description page of Project Settings.
#include "Inventory/AC_InventoryComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "Inventory/Inventory_Widget.h"
#include "Characters/CNox_Runner.h"
#include "Lootable_Base.h"

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

	ConstructorHelpers::FObjectFinder<UInputAction>TempIA_RotateItem(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_RotateItem.IA_RotateItem'"));
	if (TempIA_RotateItem.Succeeded())
	{
		IA_RotateItem = TempIA_RotateItem.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction>TempIA_LootableItem(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_LootableItem.IA_LootableItem'"));
	if (TempIA_LootableItem.Succeeded())
	{
		IA_LootableItem = TempIA_LootableItem.Object;
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

}

void UAC_InventoryComponent::SetupInputBinding(class UEnhancedInputComponent* Input)
{
	Input->BindAction(IA_InventoryMode, ETriggerEvent::Started, this, &UAC_InventoryComponent::ShowInventory);
	Input->BindAction(IA_RotateItem, ETriggerEvent::Started, this, &UAC_InventoryComponent::RotateItem);
	Input->BindAction(IA_LootableItem, ETriggerEvent::Started, this, &UAC_InventoryComponent::ShowLootableInventory);
}

void UAC_InventoryComponent::ShowInventory()
{
	if (bIsLootableMode) return;

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

void UAC_InventoryComponent::RotateItem()
{
	UDragDropOperation* CurrentOp = UWidgetBlueprintLibrary::GetDragDroppingContent();
	if (!CurrentOp) return;

	UItemObject* ItemObject = Cast<UItemObject>(CurrentOp->Payload);
	if (IsValid(ItemObject))
	{
		ItemObject->Rotate();

		UInventory_ItemWidget* ItemWidget = Cast<UInventory_ItemWidget>(CurrentOp->DefaultDragVisual);
		if (ItemWidget)
		{
			ItemWidget->ItemObject = ItemObject;
			ItemWidget->Refresh();
		}
	}
}

void UAC_InventoryComponent::ShowLootableInventory()
{
	if (bIsInventoryMode) return;

	if (bIsLootableMode) {
		bIsLootableMode = false;
		if (InventoryMainUI)
		{
			InventoryMainUI->bIsLootable = bIsLootableMode;
			InventoryMainUI->RemoveFromParent();
		}
		FInputModeGameOnly GameInputMode;
		pc->SetInputMode(GameInputMode);
		pc->bShowMouseCursor = false;
		return;
	}

	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GetOwner());

	FVector StartPos = PlayerCharacter->GetActorLocation();
	FVector ForwardVector = PlayerCharacter->GetControlRotation().Vector();
	FVector EndPos = StartPos + (ForwardVector * 300.f);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartPos, EndPos, ECC_Visibility, TraceParams);

	if (bHit)
	{
		ALootable_Base* LootableActor = Cast<ALootable_Base>(HitResult.GetActor());
		if (LootableActor)
		{
			if (!bIsLootableMode) {
				bIsLootableMode = true;
				PlayerCharacter->LootableInventoryComp = LootableActor->LootInventoryComp;
				if (InventoryWidget)
				{
					InventoryMainUI = CreateWidget<UInventory_Widget>(GetWorld(), InventoryWidget);
					InventoryMainUI->bIsLootable = bIsLootableMode;
				}
				if (InventoryMainUI)
				{
					InventoryMainUI->AddToViewport();
				}
				FInputModeGameAndUI UIInputMode;
				pc->SetInputMode(UIInputMode);
				pc->bShowMouseCursor = true;
			}
		}
	}
}
