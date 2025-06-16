#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Characters/CNox.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "CNox_Runner.generated.h"

struct FItemSaveData;
class UItemObject;
enum class EPlayerPart : uint8;
enum class EMemoryTriggerType : uint8;

UCLASS()
class BODYCREDIT_API ACNox_Runner : public ACNox, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

protected: // Modular Character Mesh
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* Hair;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* UpperBody;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* Arms;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* Backpack;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* ChestRig;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* LowerBody;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* Foot;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* Weapon1;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* Weapon2;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* TPSCamera;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCStateComponent* State;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCMovementComponent* Movement;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCWeaponComponent* Weapon;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCZoomComponent* Zoom;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCMontageComponent* Montage;

private:
	UPROPERTY(VisibleAnywhere, Category = "EnhancedInput")
	class UInputMappingContext* IMC_Movement;

	UPROPERTY(VisibleAnywhere, Category = "EnhancedInput")
	class UInputMappingContext* IMC_Weapon;

	UPROPERTY(VisibleAnywhere, Category = "EnhancedInput")
	class UInputMappingContext* IMC_Invectory;

	UPROPERTY(VisibleAnywhere, Category = "EnhancedInput")
	class UInputAction* IA_Jump;

private: // UI
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UCUserWidget_RunnerUI> RunnerUIClass;

	UPROPERTY()
	class UCUserWidget_RunnerUI* RunnerUIWidget;
	
public:
	ACNox_Runner();

protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
							 class AController* EventInstigator, AActor* DamageCauser) override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

public:
	FORCEINLINE USkeletalMeshComponent* GetHair() { return Hair; }
	FORCEINLINE USkeletalMeshComponent* GetUpperBody() { return UpperBody; }
	FORCEINLINE USkeletalMeshComponent* GetChestRig() { return ChestRig; }
	FORCEINLINE USkeletalMeshComponent* GetArms() { return Arms; }
	FORCEINLINE USkeletalMeshComponent* GetBackpack() { return Backpack; }
	FORCEINLINE USkeletalMeshComponent* GetLowerBody() { return LowerBody; }
	FORCEINLINE USkeletalMeshComponent* GetWeapon1() { return Weapon1; }
	FORCEINLINE USkeletalMeshComponent* GetWeapon2() { return Weapon2; }

	UPROPERTY(EditDefaultsOnly, Category = "Default Meshes")
	TMap<EPlayerPart, USkeletalMesh*> DefaultMeshes;

	void CacheDefaultSkeletalMeshes();

	UFUNCTION(blueprintCallable)
	void OnMovement() { Movement->Move(); };
	UFUNCTION(blueprintCallable)
	void OffMovement() {Movement->Stop(); };
	
private:
	void Init();

	FVector2D GetLastMovementInputVector2D() const;

	void Dead();

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
public:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCNoxHPComponent* HPComp;

	bool IsEscape = false;
// Inventory
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAC_InventoryComponent* InventoryComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerGold = 10000;

// Equip Item
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAC_EquipComponent* EquipComp;

// Market
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAC_MarketComponent* MarketComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneCaptureComponent2D* SceneCapture2D;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* CaptureRoot;

/**
 *	Memory Piece Add Function - LHJ (2025.05.12)
 */
public:	

	void ShowPlayerMainUI();
	void RemovePlayerMainUI();

	UFUNCTION(BlueprintCallable)
	void MakeMemoryPiece(const EMemoryTriggerType& Trigger);

	// Looting
public: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAC_LootingInventoryComponent* LootableInventoryComp;
	
	UItemObject* CreateItemFromData(const FItemSaveData& Data);

public: // Flash Bang
	UFUNCTION(BlueprintNativeEvent)
		void ReactFlashBang(FVector InLocation);
	virtual void ReactFlashBang_Implementation(FVector InLocation) {};

private: // ObserverComp
	UPROPERTY(EditDefaultsOnly)
	class UCNoxObserverComp* ObserverComp;
public:
	void RegisterAttack();
	void RegisterLooting();

public: // Card Key Check
	UFUNCTION(BlueprintCallable)
	bool CheckCardKey() const { return bHasCardKey; }
	void SetDropCardKey(bool bDrop) { bHasCardKey = bDrop; }
private:
	bool bHasCardKey = false;

public:
	FRotator TargetControlRotation;
	bool bShouldRotateToTarget = false;
	float RotationInterpSpeed = 8.f;

private:
	FTimerHandle JumpDelayHandle;
	float LastJumpInputTime = -1.0f;
	float DoubleTapThreshold = 0.18f;
	bool bPendingJump = false;

public:
	// Space(점프/회피) 입력 처리 함수
	void OnJumpOrDodgeInput();
	void DoJumpIfNoDoubleTap();
	bool IsDoubleTap();

	float CachedMoveForward = 0.f;
	float CachedMoveRight = 0.f;

	UPROPERTY(EditAnywhere, Category="Footstep")
	TArray<USoundBase*> FootstepSounds;

	UPROPERTY(EditAnywhere, Category="Footstep")
	FName LeftFootSocketName = "foot_l";
	UPROPERTY(EditAnywhere, Category="Footstep")
	FName RightFootSocketName = "foot_r";

	bool bLeftFootOnGround = false;
	bool bRightFootOnGround = false;

	void CheckFootstep(FName FootSocketName, bool& bWasOnGround);
	void PlayFootstepSound(const FVector& Location);
	
};
