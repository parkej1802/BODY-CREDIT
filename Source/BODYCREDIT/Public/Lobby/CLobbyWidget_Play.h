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
	UFUNCTION()
	void OnPlayClicked();

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Next;

};
