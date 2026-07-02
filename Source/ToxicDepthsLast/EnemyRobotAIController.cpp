// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyRobotAIController.h"
#include "EnemyRobot.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/Engine.h"

const FName AEnemyRobotAIController::TargetActorKey(TEXT("TargetActor"));

AEnemyRobotAIController::AEnemyRobotAIController()
{
}

void AEnemyRobotAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AEnemyRobot* Robot = Cast<AEnemyRobot>(InPawn);
	if (!Robot || !Robot->BehaviorTree)
	{
		
		return;
	}

	// Behavior Tree'nin kendi Blackboard asset'ini kullanarak Blackboard'ý baþlat
	UBlackboardComponent* BlackboardComp = nullptr;
	if (UseBlackboard(Robot->BehaviorTree->BlackboardAsset, BlackboardComp))
	{
		Blackboard = BlackboardComp;
		RunBehaviorTree(Robot->BehaviorTree);

		
	}
	else
	{
		
	}
}