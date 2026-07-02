// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_DetectPlayer.h"
#include "EnemyRobot.h"
#include "EnemyRobotAIController.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToxicDepthsLastCharacter.h"

UBTService_DetectPlayer::UBTService_DetectPlayer()
{
	NodeName = TEXT("Oyuncuyu Algila");

	// Her 0.2 saniyede bir kontrol et (her frame kontrol etmeye gerek yok)
	Interval = 0.2f;
	RandomDeviation = 0.05f;
}

void UBTService_DetectPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		
		return;
	}

	AEnemyRobot* Robot = Cast<AEnemyRobot>(AIController->GetPawn());
	if (!Robot)
	{
		
		return;
	}

	AToxicDepthsLastCharacter* Player = Cast<AToxicDepthsLastCharacter>(
		UGameplayStatics::GetPlayerCharacter(Robot->GetWorld(), 0));

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	

	float Distance = FVector::Dist(Robot->GetActorLocation(), Player->GetActorLocation());

	

	if (Distance <= Robot->DetectionRange)
	{
		// Oyuncu menzilde: hedef olarak ata, MoveTo baþlasýn
		BB->SetValueAsObject(AEnemyRobotAIController::TargetActorKey, Player);

		
	}
	else
	{
		// Oyuncu menzil dýþýnda: hedefi temizle, robot dursun
		BB->ClearValue(AEnemyRobotAIController::TargetActorKey);
	}
}