#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Characters/CNox.h"
#include "AC_LootingInventoryComponent.h"
#include "CNox_Runner.generated.h"

UCLASS()
class BODYCREDIT_API ACNox_Runner : public ACNox, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* TPSCamera;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* FPSCamera;

private:
	//UPROPERTY(VisibleAnywhere, Category = "Components")
	//class UCharacterTrajectoryComponent* Trajectory;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCStateComponent* State;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCMovementComponent* Movement;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCWeaponComponent* Weapon;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCZoomComponent* Zoom;

private:
	UPROPERTY(VisibleAnywhere, Category = "EnhancedInput")
	class UInputMappingContext* MappingContext;
	
public:
	ACNox_Runner();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void Init();

/**
 *	Team ID Setting - LHJ (2025.05.02)
 *	IGenericTeamAgentInterface 상속 추가
 *	GenericTeamAgentInterface.h 헤더 추가
 */
private:
	virtual FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(TeamID); }

/**
 *	HP Component - LHJ (2025.05.07)
 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCNoxHPComponent* HPComp;

// Inventory
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAC_InventoryComponent* InventoryComp;


// Equip Item
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAC_EquipComponent* EquipComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAC_MarketComponent* MarketComp;

/**
 *	Memory Piece Add Function - LHJ (2025.05.12)
 */
public:	
	UFUNCTION(BlueprintCallable)
	void MakeMemoryPiece();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAC_LootingInventoryComponent* LootableInventoryComp;


	UItemObject* CreateItemFromData(const FItemSaveData& Data);
};
