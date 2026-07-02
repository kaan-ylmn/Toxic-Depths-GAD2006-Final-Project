// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "ToxicDepthsLastCharacter.h"
#include "ToxicDepthsLastProjectile.h"
#include "EnemyRobot.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}


void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// --- Mermi kontrolü ---
	if (Character->Ammo <= 0)
	{
		// Mermi yok, ateþ etme
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(30, 1.5f, FColor::Red, TEXT("Mermi yok!"));
		}
		return;
	}

	// Mermi var, bir tane harca
	Character->Ammo -= 1;

	// --- HITSCAN ateþ (line trace) ---
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	if (PlayerController)
	{
		// Kameranýn konumu ve baktýðý yön
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		FVector TraceStart = CameraLocation;
		FVector TraceEnd = CameraLocation + (CameraRotation.Vector() * WeaponRange);

		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Character);   // kendimize vurmayalým
		Params.AddIgnoredActor(GetOwner());

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult, TraceStart, TraceEnd, ECC_Visibility, Params);

		// Merminin gittiði çizgiyi göster (test için, sarý çizgi)
		DrawDebugLine(GetWorld(), TraceStart, bHit ? HitResult.Location : TraceEnd,
			FColor::Yellow, false, 0.2f, 0, 1.0f);

		if (bHit && HitResult.GetActor())
		{
			// Ýsabet noktasýnda küçük bir iþaret (test için)
			DrawDebugSphere(GetWorld(), HitResult.Location, 8.0f, 8, FColor::Red, false, 0.5f);

			// Vurduðumuz þey bir robot mu?
			AEnemyRobot* Robot = Cast<AEnemyRobot>(HitResult.GetActor());
			if (Robot)
			{
				// Robota hasar ver
				Robot->TakeRobotDamage(WeaponDamage);
			}
			else
			{
				// Robot deðilse sadece ne vurduðumuzu yaz
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(31, 1.5f, FColor::Green,
						FString::Printf(TEXT("Vurdun: %s"), *HitResult.GetActor()->GetName()));
				}
			}
		}
	}

	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}

	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

bool UTP_WeaponComponent::AttachWeapon(AToxicDepthsLastCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UTP_WeaponComponent>())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
		}
	}

	return true;
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}