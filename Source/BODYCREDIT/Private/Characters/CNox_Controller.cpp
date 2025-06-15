#include "Characters/CNox_Controller.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h"
#include "Inventory/Inventory_ItemWidget.h"
#include "Inventory/Inventory_EquipmentTile.h"
#include "Item/ItemObject.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Characters/CNox_Runner.h"
#include "Session/NetGameInstance.h"
#include "Global.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

ACNox_Controller::ACNox_Controller()
{
	CHelpers::CreateComponent<UAudioComponent>(this, &BGMAudioComponent, "BGMAudioComponent", RootComponent);
}

void ACNox_Controller::SetupInputComponent()
{
    Super::SetupInputComponent();

    // InputComponent->BindKey(EKeys::R, IE_Pressed, this, &ACNox_Controller::RotateDraggedItem);
}

void ACNox_Controller::RotateDraggedItem()
{
    UDragDropOperation* DragOp = UWidgetBlueprintLibrary::GetDragDroppingContent();
    if (!DragOp) return;

    UItemObject* ItemObj = Cast<UItemObject>(DragOp->Payload);
    if (!IsValid(ItemObj)) return;

    ItemObj->Rotate();

    if (UInventory_ItemWidget* ItemWidget = Cast<UInventory_ItemWidget>(DragOp->DefaultDragVisual))
    {
        ItemWidget->ItemObject = ItemObj;
        ItemWidget->Refresh();
    }

    if (UInventory_EquipmentTile* EquipTile = Cast<UInventory_EquipmentTile>(DragOp->DefaultDragVisual))
    {
        EquipTile->ItemObject = ItemObj;
        EquipTile->Refresh();
    }

	// GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, TEXT("Item Rotated (R key)"));
}

void ACNox_Controller::PlayBGM(const int32 idx)
{
	if (UNetGameInstance* GameInstance = Cast<UNetGameInstance>(GetGameInstance()))
	{
		if (USoundCue* BGM = GameInstance->GetBGM())
		{
			// 현재 재생 중인 BGM과 같은 idx가 아닌 경우에만 중지
			if (CurrentBGMIndex != idx)
			{
				StopBGM();
			}
			else
			{
				return; // 같은 BGM이 재생 중이면 아무것도 하지 않음
			}

			// SoundCue 설정
			BGMAudioComponent->SetSound(BGM);
			
			// idx 파라미터 설정
			BGMAudioComponent->SetIntParameter(FName("Idx"), idx);
			
			// 볼륨 설정
			// BGMAudioComponent->SetVolumeMultiplier(1.0f);
			
			// 현재 재생 중인 BGM의 idx 저장
			CurrentBGMIndex = idx;
			
			// 재생
			BGMAudioComponent->Play();
		}
	}
}

void ACNox_Controller::PauseBGM(bool bIsPause)
{
	if (BGMAudioComponent)
	{
		BGMAudioComponent->SetPaused(bIsPause);
	}
}

void ACNox_Controller::StopBGM()
{
	if (BGMAudioComponent)
	{
		BGMAudioComponent->Stop();
		CurrentBGMIndex = -1; // 재생 중인 BGM이 없음을 표시
	}
}