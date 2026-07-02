// Copyright Epic Games, Inc. All Rights Reserved.

#include "PickupItem.h"
#include "Components/StaticMeshComponent.h"

APickupItem::APickupItem()
{
	PrimaryActorTick.bCanEverTick = false;

	// Mesh'i oluþtur ve kök bileþen yap
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
}

void APickupItem::Collect()
{
	// Þimdilik sadece kendini yok et; sayacý artýrmayý karakter tarafýnda yapacaðýz
	Destroy();
}