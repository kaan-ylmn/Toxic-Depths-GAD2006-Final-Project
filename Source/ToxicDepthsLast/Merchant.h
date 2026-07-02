// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Merchant.generated.h"

UCLASS()
class TOXICDEPTHSLAST_API AMerchant : public AActor
{
	GENERATED_BODY()

public:
	AMerchant();

	// Satýcýnýn görünür gövdesi
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Merchant")
	UStaticMeshComponent* MeshComp;

	// --- Fiyatlar (her alýmda artacak) ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Merchant|Prices")
	int32 OxygenTankPrice = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Merchant|Prices")
	int32 PartPrice = 25;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Merchant|Prices")
	int32 MedkitPrice = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Merchant|Prices")
	int32 AmmoPrice = 10;

	// --- Her alýmda fiyata eklenecek zam miktarý ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Merchant|Prices")
	int32 PriceIncrease = 10;

	// Parça satýnca oyuncunun kazanacaðý altýn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Merchant|Prices")
	int32 PartSellValue = 15;
};