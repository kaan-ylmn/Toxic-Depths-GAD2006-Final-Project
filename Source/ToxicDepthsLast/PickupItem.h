// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItem.generated.h"

// Bu nesne ne veriyor: altýn mý, parça mý?
UENUM(BlueprintType)
enum class EPickupType : uint8
{
	Gold	UMETA(DisplayName = "Gold"),
	Part	UMETA(DisplayName = "Part")
};

UCLASS()
class TOXICDEPTHSLAST_API APickupItem : public AActor
{
	GENERATED_BODY()

public:
	APickupItem();

	// Görünür gövde (küp, küre vb.)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	UStaticMeshComponent* MeshComp;

	// Bu nesne altýn mý parça mý veriyor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	EPickupType PickupType = EPickupType::Gold;

	// Ne kadar veriyor (örn. 10 altýn, 1 parça)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	int32 Amount = 10;

	// Oyuncu bu nesneyi topladýðýnda çaðrýlýr
	void Collect();
};