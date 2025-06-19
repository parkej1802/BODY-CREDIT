#include "Lobby/Tutorial/CGuideWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

namespace
{
    const FLinearColor SelectedColor(0.2f, 0.2f, 0.5f, 1.0f);
    const FLinearColor DefaultColor(0.f, 0.f, 0.5f, 1.0f);
}

void UCGuideWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Btn_GameDescription)
    {
        Btn_GameDescription->OnClicked.RemoveDynamic(this, &UCGuideWidget::OnGameDescClicked);
        Btn_GameDescription->OnClicked.AddDynamic(this, &UCGuideWidget::OnGameDescClicked);
    }
    if (Btn_Key)
    {
        Btn_Key->OnClicked.RemoveDynamic(this, &UCGuideWidget::OnKeyClicked);
        Btn_Key->OnClicked.AddDynamic(this, &UCGuideWidget::OnKeyClicked);
    }
    if (Btn_Player)
    {
        Btn_Player->OnClicked.RemoveDynamic(this, &UCGuideWidget::OnPlayerClicked);
        Btn_Player->OnClicked.AddDynamic(this, &UCGuideWidget::OnPlayerClicked);
    }
    if (Btn_Inventory)
    {
        Btn_Inventory->OnClicked.RemoveDynamic(this, &UCGuideWidget::OnInventoryClicked);
        Btn_Inventory->OnClicked.AddDynamic(this, &UCGuideWidget::OnInventoryClicked);
    }
    if (Btn_Enemy)
    {
        Btn_Enemy->OnClicked.RemoveDynamic(this, &UCGuideWidget::OnEnemyClicked);
        Btn_Enemy->OnClicked.AddDynamic(this, &UCGuideWidget::OnEnemyClicked);
    }
    if (Btn_Map)
    {
        Btn_Map->OnClicked.RemoveDynamic(this, &UCGuideWidget::OnMapClicked);
        Btn_Map->OnClicked.AddDynamic(this, &UCGuideWidget::OnMapClicked);
    }
    if (Btn_Prev)
    {
        Btn_Prev->OnClicked.RemoveDynamic(this, &UCGuideWidget::OnPrevClicked);
        Btn_Prev->OnClicked.AddDynamic(this, &UCGuideWidget::OnPrevClicked);
    }
    if (Btn_Later)
    {
        Btn_Later->OnClicked.RemoveDynamic(this, &UCGuideWidget::OnLaterClicked);
        Btn_Later->OnClicked.AddDynamic(this, &UCGuideWidget::OnLaterClicked);
    }

    SetGuideArray(GameDescArray, Btn_GameDescription);
    SetButtonColor(Btn_GameDescription);
}

void UCGuideWidget::SetButtonColor(UButton* SelectedBtn)
{
    TArray<UButton*> AllBtns = {Btn_GameDescription, Btn_Key, Btn_Player, Btn_Inventory, Btn_Enemy, Btn_Map};
    for (UButton* Btn : AllBtns)
    {
        if (!Btn) continue;
        FButtonStyle Style = Btn->GetStyle();
        FLinearColor Color = (Btn == SelectedBtn) ? SelectedColor : DefaultColor;
        Style.Normal.TintColor = Color;
        Style.Hovered.TintColor = Color;
        Style.Pressed.TintColor = Color;
        Btn->SetStyle(Style);
    }
}

void UCGuideWidget::SetGuideArray(const TArray<UTexture2D*>& InArray, UButton* SelectedBtn)
{
    CurrentArray = &InArray;
    GuideIndex = 0;
    MaxPage = InArray.Num();
    CurPage = 1;
    UpdateGuide();
    SetButtonColor(SelectedBtn);
}

void UCGuideWidget::UpdateGuide()
{
    if (!CurrentArray || !Img_Guide) return;
    if (CurrentArray->IsValidIndex(GuideIndex))
        Img_Guide->SetBrushFromTexture((*CurrentArray)[GuideIndex]);
    if (Txt_Page)
        Txt_Page->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurPage, MaxPage)));
}

void UCGuideWidget::OnGameDescClicked()     { SetGuideArray(GameDescArray, Btn_GameDescription); }
void UCGuideWidget::OnKeyClicked()         { SetGuideArray(KeyGuideArray, Btn_Key); }
void UCGuideWidget::OnPlayerClicked()      { SetGuideArray(PlayerGuideArray, Btn_Player); }
void UCGuideWidget::OnInventoryClicked()   { SetGuideArray(InventoryGuideArray, Btn_Inventory); }
void UCGuideWidget::OnEnemyClicked()       { SetGuideArray(EnemyGuideArray, Btn_Enemy); }
void UCGuideWidget::OnMapClicked()         { SetGuideArray(MapGuideArray, Btn_Map); }

void UCGuideWidget::OnPrevClicked()
{
    if (GuideIndex > 0)
    {
        GuideIndex--;
        CurPage = GuideIndex + 1;
        UpdateGuide();
    }
}

void UCGuideWidget::OnLaterClicked()
{
    if (GuideIndex < MaxPage - 1)
    {
        GuideIndex++;
        CurPage = GuideIndex + 1;
        UpdateGuide();
    }
}

