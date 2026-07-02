// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_DetectPlayer.generated.h"

/**
 * Her tick oyuncuyla robot arasýndaki mesafeyi ölçer.
 * Oyuncu DetectionRange içindeyse Blackboard'daki TargetActor'a yazar,
 * dýþýndaysa temizler (robot MoveTo yapmayý býrakýr).
 */
UCLASS()
class TOXICDEPTHSLAST_API UBTService_DetectPlayer : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_DetectPlayer();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};