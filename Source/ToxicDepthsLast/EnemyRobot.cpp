// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyRobot.h"
#include "EnemyRobotAIController.h"
#include "Engine/Engine.h"
#include "ToxicDepthsLastCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemyRobot::AEnemyRobot()
{
	// Set this character to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	// Bu robotu bizim AI Controller'ýmýz kontrol etsin
	AIControllerClass = AEnemyRobotAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Robot hareket ederken baktýðý yöne deðil, gittiði yöne dönsün
	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	}
}

// Called when the game starts or when spawned
void AEnemyRobot::BeginPlay()
{
	Super::BeginPlay();

	// Baþlangýçta caný doldur
	Health = MaxHealth;
}

// Called every frame
void AEnemyRobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Saldýrý zamanlayýcýsýný ilerlet
	TimeSinceLastAttack += DeltaTime;

	// Oyuncuyu bul
	AToxicDepthsLastCharacter* Player = Cast<AToxicDepthsLastCharacter>(
		UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Player)
	{
		// Robot ile oyuncu arasýndaki mesafe
		float Distance = FVector::Dist(GetActorLocation(), Player->GetActorLocation());

		// Menzildeyse ve cooldown dolduysa saldýr
		if (Distance <= AttackRange && TimeSinceLastAttack >= AttackCooldown)
		{
			TimeSinceLastAttack = 0.0f; // sayacý sýfýrla

			// Oyuncunun canýný azalt
			Player->Health -= AttackDamage;

			// Test için ekrana yaz
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1, 1.0f, FColor::Purple,
					FString::Printf(TEXT("Robot sana vurdu! -%.0f can"), AttackDamage));
			}
		}
	}
}

// Called to bind functionality to input
void AEnemyRobot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Silahtan hasar alýnca
void AEnemyRobot::TakeRobotDamage(float DamageAmount)
{
	Health -= DamageAmount;

	// Test için ekrana yaz
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 1.5f, FColor::Orange,
			FString::Printf(TEXT("Robot hasar aldi! Kalan can: %.0f"), Health));
	}

	// Can bittiyse öl
	if (Health <= 0.0f)
	{
		Die();
	}
}

// Robot ölünce
void AEnemyRobot::Die()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 3.0f, FColor::Red, TEXT("Robot oldu!"));
	}

	// Robotu sahneden kaldýr (yok et)
	Destroy();
}