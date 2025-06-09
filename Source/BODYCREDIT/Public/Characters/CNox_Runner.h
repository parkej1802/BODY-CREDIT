#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Characters/CNox.h"
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
	class USkeletalMeshComponent* Clothes;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* Arms;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* LowerBody;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* Foot;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* TPSCamera;

	//UPROPERTY(VisibleAnywhere, Category = "Camera")
	//class UCameraComponent* FPSCamera;

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
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
							 class AController* EventInstigator, AActor* DamageCauser) override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FORCEINLINE USkeletalMeshComponent* GetHead() { return GetMesh(); }
	FORCEINLINE USkeletalMeshComponent* GetUpperBody() { return UpperBody; }
	FORCEINLINE USkeletalMeshComponent* GetArms() { return Arms; }
	FORCEINLINE USkeletalMeshComponent* GetLowerBody() { return LowerBody; }

	UPROPERTY(EditDefaultsOnly, Category = "Default Meshes")
	TMap<EPlayerPart, USkeletalMesh*> DefaultMeshes;

	void CacheDefaultSkeletalMeshes();
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
public:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCNoxHPComponent* HPComp;

// Inventory
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAC_InventoryComponent* InventoryComp;


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
};
