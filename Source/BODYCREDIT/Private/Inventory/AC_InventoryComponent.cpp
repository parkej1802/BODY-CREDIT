// Fill out your copyright notice in the Description page of Project Settings.
#include "Inventory/AC_InventoryComponent.h"
#include "Global.h"
#include "AC_LootingInventoryComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "Inventory/Inventory_Widget.h"
#include "Characters/CNox_Runner.h"
#include "Lootable_Base.h"
#include "Lobby/LobbyWidget_Pause.h"
#include "Inventory/Inventory_EquipmentTile.h"
#include "Characters/CNox_Controller.h"
#include "Inventory/AC_EquipComponent.h"
#include "Item/Item_Base.h"
#include "Components/CMovementComponent.h"
#include "Camera/CameraComponent.h"

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

	ConstructorHelpers::FObjectFinder<UInputAction>TempIA_PauseGame(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Pause.IA_Pause'"));
	if (TempIA_PauseGame.Succeeded())
	{
		IA_Pause = TempIA_PauseGame.Object;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> TempWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Inventory/WBP_MainInventory.WBP_MainInventory'"));
	if (TempWidget.Succeeded())
	{
		InventoryWidget = TempWidget.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> TempPauseWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Lobby/UI/WBP_LobbyPause.WBP_LobbyPause'"));
	if (TempPauseWidget.Succeeded())
	{
		PauseGameWidget = TempPauseWidget.Class;
	}
}

// Called when the game starts
void UAC_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();

	if ((pc = OwnerActor->GetInstigatorController<ACNox_Controller>()) != nullptr)
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
	Input->BindAction(IA_Pause, ETriggerEvent::Started, this, &UAC_InventoryComponent::PauseGame);
}

void UAC_InventoryComponent::ShowInventory()
{

	if (InventoryMainUI)
	{
		InventoryMainUI->RemoveFromParent();
		InventoryMainUI = nullptr;
		bIsLootableMode = false;
		bIsInventoryMode = false;
		FInputModeGameOnly GameInputMode;
		pc->SetInputMode(GameInputMode);
		pc->bShowMouseCursor = false;
		return;
	}

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
			InventoryMainUI = nullptr;
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
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("RotateItem"));
			ItemWidget->ItemObject = ItemObject;
			ItemWidget->Refresh();
		}
		UInventory_EquipmentTile* Tile = Cast<UInventory_EquipmentTile>(CurrentOp->DefaultDragVisual);
		if (Tile)
		{
			Tile->Refresh();
		}
	}
}

void UAC_InventoryComponent::ShowLootableInventory()
{

	if (InventoryMainUI)
	{
		InventoryMainUI->RemoveFromParent();
		InventoryMainUI = nullptr;
		bIsLootableMode = false;
		bIsInventoryMode = false;
		FInputModeGameOnly GameInputMode;
		pc->SetInputMode(GameInputMode);
		pc->bShowMouseCursor = false;
		CHelpers::GetComponent<UCMovementComponent>(PlayerCharacter)->Move();
		return;
	}

	if (bIsInventoryMode) return;

	if (bIsLootableMode) {
		bIsLootableMode = false;
		if (InventoryMainUI)
		{
			InventoryMainUI->bIsLootable = bIsLootableMode;
			InventoryMainUI->RemoveFromParent();
			InventoryMainUI = nullptr;
		}
		FInputModeGameOnly GameInputMode;
		pc->SetInputMode(GameInputMode);
		pc->bShowMouseCursor = false;
		CHelpers::GetComponent<UCMovementComponent>(PlayerCharacter)->Move();
		return;
	}

	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GetOwner());

	FVector StartPos = CHelpers::GetComponent<UCameraComponent>(PlayerCharacter)->GetComponentLocation();
	FVector ForwardVector = CHelpers::GetComponent<UCameraComponent>(PlayerCharacter)->GetForwardVector();
	FVector EndPos = StartPos + (ForwardVector * 400.f);

	//DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Red);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartPos, EndPos, ECC_Visibility, TraceParams);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (!HitActor) return;

		AItem_Base* ItemActor = Cast<AItem_Base>(HitActor);


		if (ItemActor) {
			if (PlayerCharacter->EquipComp->EquippedItems[EPlayerPart::Backpack])
			{
				if (PlayerCharacter->EquipComp->EquippedItems[EPlayerPart::Backpack]->ItemActorOwner->LootInventoryComp->TryAddItem(ItemActor->ItemObject)) 
				{
					ItemActor->SetActorHiddenInGame(true);
					ItemActor->SetActorEnableCollision(false);
					return;
				}
			}
			else if (PlayerCharacter->EquipComp->EquippedItems[EPlayerPart::ChestRigs])
			{
				if (PlayerCharacter->EquipComp->EquippedItems[EPlayerPart::ChestRigs]->ItemActorOwner->LootInventoryComp->TryAddItem(ItemActor->ItemObject))
				{
					ItemActor->SetActorHiddenInGame(true);
					ItemActor->SetActorEnableCollision(false);
					return;
				}
			}
		}
		else
		{
			// 액터의 블루프린트에 있는 함수를 가져와서 있다면 호출한다.
			UFunction* InteractFunction = HitActor->FindFunction(FName("StartExtractWave"));
			if (InteractFunction && InteractFunction->HasAnyFunctionFlags(FUNC_BlueprintCallable))
			{
				struct {
					ACNox_Runner* Player;  // 함수 시그니처에 맞는 변수
				} ExtractParam;
				ExtractParam.Player = PlayerCharacter;
				HitActor->ProcessEvent(InteractFunction, &ExtractParam);
				return;
			}
		}
		
		UAC_LootingInventoryComponent* LootComp = HitActor->FindComponentByClass<UAC_LootingInventoryComponent>();

		if (!LootComp) return;

		if (LootComp && !bIsLootableMode)
		{
			bIsLootableMode = true;
			PlayerCharacter->LootableInventoryComp = LootComp;

			if (InventoryWidget)
			{
				InventoryMainUI = CreateWidget<UInventory_Widget>(GetWorld(), InventoryWidget);
				InventoryMainUI->bIsLootable = true;
			}
			if (InventoryMainUI)
			{
				InventoryMainUI->AddToViewport();
				CHelpers::GetComponent<UCMovementComponent>(PlayerCharacter)->Stop();
			}

			FInputModeGameAndUI UIInputMode;
			pc->SetInputMode(UIInputMode);
			pc->bShowMouseCursor = true;

			PlayerCharacter->RegisterLooting();
		}
	}
}

void UAC_InventoryComponent::PauseGame()
{
	if (bIsPauseMode) {
		bIsPauseMode = false;
		if (PauseGameUI)
		{
			PauseGameUI->RemoveFromParent();
			PauseGameUI = nullptr;
		}

		FInputModeGameOnly GameInputMode;
		pc->SetInputMode(GameInputMode);
		pc->bShowMouseCursor = false;
		return;
	}

	if (!bIsPauseMode) {
		bIsPauseMode = true;
		if (PauseGameWidget) {
			PauseGameUI = CreateWidget<ULobbyWidget_Pause>(GetWorld(), PauseGameWidget);
		}
		if (PauseGameUI)
		{
			PauseGameUI->AddToViewport();
			CHelpers::GetComponent<UCMovementComponent>(PlayerCharacter)->Stop();
		}
		FInputModeGameAndUI UIInputMode;
		pc->SetInputMode(UIInputMode);
		pc->bShowMouseCursor = true;
	}
}
