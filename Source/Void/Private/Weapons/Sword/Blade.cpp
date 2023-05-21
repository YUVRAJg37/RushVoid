// Fill out your copyright notice in the Description page of Project Settings.


#include "Void/Public/Weapons/Sword/Blade.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

ABlade::ABlade()
{
	PrimaryActorTick.bCanEverTick = true;

	BladeMesh = CreateDefaultSubobject<UStaticMeshComponent>("Blade Mesh");
	BladeCollision = CreateDefaultSubobject<UCapsuleComponent>("Blade Collision");

	RootComponent = BladeMesh;
	BladeCollision->SetupAttachment(GetRootComponent());

	BladeMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void ABlade::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABlade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

