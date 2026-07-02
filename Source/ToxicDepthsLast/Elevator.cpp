// Copyright Epic Games, Inc. All Rights Reserved.

#include "Elevator.h"
#include "Components/StaticMeshComponent.h"
#include "ToxicDepthsLastCharacter.h"
#include "Engine/Engine.h"

AElevator::AElevator()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
}

bool AElevator::DeliverPart(AToxicDepthsLastCharacter* Player)
{
	if (!Player) { return false; }

	// Zaten tamamlandýysa bir þey yapma
	if (PartsDelivered >= PartsRequired) { return false; }

	// Oyuncunun parçasý yoksa reddet
	if (Player->Parts <= 0)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(9, 2.0f, FColor::Red, TEXT("Parcan yok!"));
		}
		return false;
	}

	// Parçayý al, sayacý artýr
	Player->Parts -= 1;
	PartsDelivered += 1;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			9, 2.0f, FColor::Green,
			FString::Printf(TEXT("Parca teslim edildi! (%d/%d)"), PartsDelivered, PartsRequired));
	}

	// 5 parça tamamlandýysa Blueprint tarafýndaki OnElevatorReady olayýný tetikle
	if (PartsDelivered >= PartsRequired)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(10, 5.0f, FColor::Cyan, TEXT("ASANSOR HAZIR! Yukseliyor..."));
		}
		OnElevatorReady();
	}

	return true;
}