// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "VoidCharacter.generated.h"


class UVoidCharacterMovementComponent;
class ABlade;

UENUM(BlueprintType)
enum class ECharacterState : uint8 
{
	ECS_Idle UMETA(DisplayName = "Idle"),
	ECS_Walking UMETA(DisplayName = "Walking"),
	ECS_Running UMETA(DisplayName = "Running"),
	ECS_Jumping UMETA(DisplayName = "Jumping"),
	ECS_Dash UMETA(DisplayName = "Dash")
};

UENUM(BlueprintType)
enum class EWeaponState : uint8 
{
	EWS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EWS_Reloading UMETA(DisplayName = "Reloading"),

	ECS_MAX UMETA(DisplayName = "Default")
};

UCLASS(config=Game)
class AVoidCharacter : public ACharacter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DashAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* EquipSwordAction;


public:
	
	AVoidCharacter();
	virtual void Tick(float DeltaSeconds) override;

protected:
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay();
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dash, meta = (AllowPrivateAccess = "true"))
	bool bChangeState;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dash, meta = (AllowPrivateAccess = "true"))
	float DashCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dash, meta = (AllowPrivateAccess = "true"))
	float DashTime;
	bool bCanDash;
	FTimerHandle DashTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dash, meta = (AllowPrivateAccess = "true"))
	float DashThreshold;
	
	UPROPERTY()
	ABlade* Blade;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dash, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABlade> BladeClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Effects, meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent* NcDashLine; 
		
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UVoidCharacterMovementComponent* VoidCharacterMovementComponent;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void Jump();
	void StopJump();

	void Dash();
	
	void HolsterSword();
	void EquipSword(ABlade* bladeToEquip);
	ABlade* SpawnSword();
	bool bSwordEquip;

private:
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Misc")
	ECharacterState CharacterState;
	
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

