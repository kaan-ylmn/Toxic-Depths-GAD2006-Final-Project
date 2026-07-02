// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Elevator.generated.h"

class AToxicDepthsLastCharacter;

UCLASS()
class TOXICDEPTHSLAST_API AElevator : public AActor
{
	GENERATED_BODY()

public:
	AElevator();

	// Asansörün görünür gövdesi (kabin)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Elevator")
	UStaticMeshComponent* MeshComp;

	// Teslim edilen parça sayýsý
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elevator")
	int32 PartsDelivered = 0;

	// Zafer için gereken toplam parça
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elevator")
	int32 PartsRequired = 5;

	// Oyuncu bir parça teslim etmeye çalýþýyor — döndürür: teslim baþarýlý mý?
	UFUNCTION(BlueprintCallable, Category = "Elevator")
	bool DeliverPart(AToxicDepthsLastCharacter* Player);

	// Asansör tamamlandýðýnda (5 parça teslim edildiðinde) Blueprint tarafýnda tetiklenecek olay
	// Blueprint'te bunu Timeline'a baðlayýp yükseliþi baþlatacaðýz
	UFUNCTION(BlueprintImplementableEvent, Category = "Elevator")
	void OnElevatorReady();
};