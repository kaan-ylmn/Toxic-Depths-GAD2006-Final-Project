// Copyright Epic Games, Inc. All Rights Reserved.

#include "Merchant.h"
#include "Components/StaticMeshComponent.h"

AMerchant::AMerchant()
{
	PrimaryActorTick.bCanEverTick = false;

	// Mesh'i oluþtur ve kök bileþen yap
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
}