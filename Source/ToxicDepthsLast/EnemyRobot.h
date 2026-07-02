// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyRobot.generated.h"

class UBehaviorTree;

UCLASS()
class TOXICDEPTHSLAST_API AEnemyRobot : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyRobot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Silahtan hasar alýnca çaðrýlýr
	void TakeRobotDamage(float DamageAmount);

	// Robot öldüðünde çaðrýlýr
	void Die();

	// AI Controller'ýn kullanacaðý Behavior Tree asset'i
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth = 100.0f;

	// Oyuncuyu bu mesafede algýlar (BTService_DetectPlayer tarafýndan kullanýlýr)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float DetectionRange = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackCooldown = 1.5f;

private:
	float TimeSinceLastAttack = 0.0f;
};