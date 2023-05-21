// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "VoidCharacterMovementComponent.generated.h"

/**
 * 
 */

class AVoidCharacter;

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None			UMETA(Hidden),
	CMOVE_Dash			UMETA(DisplayName = "Dash"),
	CMOVE_Slide			UMETA(DisplayName = "Slide"),
	CMOVE_WallRun		UMETA(DisplayName = "Wall Run"),
	CMOVE_MAX			UMETA(Hidden),
};

UCLASS()
class VOID_API UVoidCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	UVoidCharacterMovementComponent();
	
protected:

	virtual void InitializeComponent() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:

	UPROPERTY(EditDefaultsOnly) AVoidCharacter* VoidCharacter;

	//Dash

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dash, meta = (AllowPrivateAccess = "true"))
	float DashCoolDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dash, meta = (AllowPrivateAccess = "true"))
	float DashTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dash, meta = (AllowPrivateAccess = "true"))
	float DashForce;
	FTimerHandle DashTimerHandle;
	bool bCanDash;
	
	bool CanDash();
	void DashEnd();
	void ResetDash();

public:
	void InitiateDash();
	
};
