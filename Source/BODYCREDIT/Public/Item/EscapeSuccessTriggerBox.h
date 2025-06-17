// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EscapeSuccessTriggerBox.generated.h"

UCLASS()
class BODYCREDIT_API AEscapeSuccessTriggerBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEscapeSuccessTriggerBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere)
    class UBoxComponent* TriggerBox;

	void HandleEscape();

	FTimerHandle EscapeTimerHandle;
	bool bIsPlayerInside;

	UPROPERTY()
	class ACNox_Runner* CachedPlayer = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> LobbySuccessWidgetClass;

	UPROPERTY()
	class ULobbyWidget_Success* SuccessUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> EscapeTimerWidgetClass;

	UPROPERTY()
	class ULobbyWidget_EscapeTimer* EscapeTimerUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EscapeTime = 5.f;

	UPROPERTY()
	class UNetGameInstance* GI;
};
