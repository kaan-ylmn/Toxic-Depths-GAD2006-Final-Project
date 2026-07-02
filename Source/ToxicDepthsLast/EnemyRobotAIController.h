// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyRobotAIController.generated.h"

/**
 * EnemyRobot'un Behavior Tree'sini çalýþtýran AI Controller.
 */
UCLASS()
class TOXICDEPTHSLAST_API AEnemyRobotAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyRobotAIController();

	virtual void OnPossess(APawn* InPawn) override;

	// Blackboard'da oyuncuyu tutan anahtarýn adý (Object tipi, base class: Actor)
	static const FName TargetActorKey;
};