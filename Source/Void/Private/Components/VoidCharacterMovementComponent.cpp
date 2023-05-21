// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/VoidCharacterMovementComponent.h"

#include "Camera/CameraComponent.h"
#include "Void/VoidCharacter.h"

UVoidCharacterMovementComponent::UVoidCharacterMovementComponent() :
DashCoolDown(1),
DashTime(0.1),
DashForce(8000),
bCanDash(true)
{
	VoidCharacter = Cast<AVoidCharacter>(GetOwner());
}

void UVoidCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	VoidCharacter = Cast<AVoidCharacter>(GetOwner());
}

void UVoidCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UVoidCharacterMovementComponent::CanDash()
{
	return bCanDash;
}

void UVoidCharacterMovementComponent::InitiateDash()
{
	if(!CanDash())
		return;
	FVector launchDir;

	bCanDash = false;
	GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &UVoidCharacterMovementComponent::DashEnd, DashTime, false);

	float CameraAndCharacterInSameDirection = FVector::DotProduct(VoidCharacter->GetActorForwardVector(),
		VoidCharacter->GetFollowCamera()->GetForwardVector());

	if(CameraAndCharacterInSameDirection < 0)
		launchDir = VoidCharacter->GetActorForwardVector();
	else
		launchDir = VoidCharacter->GetFollowCamera()->GetForwardVector();

	VoidCharacter->LaunchCharacter(launchDir*DashForce, false, false);
}

void UVoidCharacterMovementComponent::DashEnd()
{
	VoidCharacter->GetCharacterMovement()->StopMovementImmediately();
	GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &UVoidCharacterMovementComponent::ResetDash, DashCoolDown, false);
}

void UVoidCharacterMovementComponent::ResetDash()
{
	bCanDash = true;
	GetWorld()->GetTimerManager().ClearTimer(DashTimerHandle);
}
