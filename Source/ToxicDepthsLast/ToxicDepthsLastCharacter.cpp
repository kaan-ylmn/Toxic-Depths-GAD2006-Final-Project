// Copyright Epic Games, Inc. All Rights Reserved.

#include "ToxicDepthsLastCharacter.h"
#include "ToxicDepthsLastProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "DrawDebugHelpers.h"
#include "PickupItem.h"
#include "Merchant.h"
#include "Blueprint/UserWidget.h"
#include "Elevator.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AToxicDepthsLastCharacter

AToxicDepthsLastCharacter::AToxicDepthsLastCharacter()
{
	// Bu aktörün her karede Tick almasýný saðla
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

void AToxicDepthsLastCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// --- Tutorial: baþlangýçta kontrol bilgilerini göster (8 saniye) ---
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			20, 8.0f, FColor::Green, TEXT("=== KONTROLLER ==="));
		GEngine->AddOnScreenDebugMessage(
			21, 8.0f, FColor::White, TEXT("WASD - Hareket  |  Fare - Bak"));
		GEngine->AddOnScreenDebugMessage(
			22, 8.0f, FColor::White, TEXT("E - Etkilesim (topla / satici / asansor)"));
		GEngine->AddOnScreenDebugMessage(
			23, 8.0f, FColor::White, TEXT("F - Oksijen Tupu Kullan  |  G - Medkit Kullan"));
		GEngine->AddOnScreenDebugMessage(
			24, 8.0f, FColor::Yellow, TEXT("Amac: 5 parca topla, asansoru tamir et, kac!"));
	}
}

void AToxicDepthsLastCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Öldüyse istatistikleri güncellemeye devam etme
	if (bIsDead)
	{
		return;
	}

	// Oksijeni bulunduðumuz yere göre azalt (hýz trigger'larla deðiþir)
	Oxygen -= OxygenDrainRate * DeltaTime;

	// 0'ýn altýna düþmesin
	if (Oxygen < 0.0f)
	{
		Oxygen = 0.0f;
	}

	// Ekranda durum bilgisi (test için)
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			1, 0.0f, FColor::Cyan,
			FString::Printf(TEXT("Oksijen: %.0f  (hiz: %.0f)"), Oxygen, OxygenDrainRate));

		GEngine->AddOnScreenDebugMessage(
			3, 0.0f, FColor::Yellow,
			FString::Printf(TEXT("Altin: %d   Parca: %d"), Gold, Parts));

		GEngine->AddOnScreenDebugMessage(
			5, 0.0f, FColor::White,
			FString::Printf(TEXT("Tup: %d   Medkit: %d   Mermi: %d"), OxygenTanks, Medkits, Ammo));

		GEngine->AddOnScreenDebugMessage(
			9, 0.0f, FColor::Red,
			FString::Printf(TEXT("Can: %.0f"), Health));
	}

	// Oksijen bittiyse, ölme yerine cana hasar ver (saniyede 5)
	if (Oxygen <= 0.0f)
	{
		Health -= 5.0f * DeltaTime;
	}

	// Can bittiyse de öl
	if (Health <= 0.0f)
	{
		OnDeath();
	}
}

void AToxicDepthsLastCharacter::OnDeath()
{
	// Zaten öldüyse tekrar çalýþmasýn (widget'ý iki kere açmayalým)
	if (bIsDead)
	{
		return;
	}
	bIsDead = true;

	// Oyuncunun hareketini durdur (öldü)
	DisableInput(nullptr);

	// Açýk duran alýþveriþ menüsü varsa kapat
	if (ShopMenuWidget)
	{
		CloseShopMenu();
	}

	// Yenilgi ekranýný oluþtur ve göster
	if (DefeatScreenClass)
	{
		DefeatScreenWidget = CreateWidget<UUserWidget>(GetWorld(), DefeatScreenClass);
		if (DefeatScreenWidget)
		{
			DefeatScreenWidget->AddToViewport();

			if (APlayerController* PC = Cast<APlayerController>(GetController()))
			{
				PC->SetShowMouseCursor(true);

				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(DefeatScreenWidget->TakeWidget());
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PC->SetInputMode(InputMode);
			}
		}
	}
	else if (GEngine)
	{
		// DefeatScreenClass henüz atanmadýysa test için ekrana yaz
		GEngine->AddOnScreenDebugMessage(
			2, 5.0f, FColor::Red, TEXT("OLDUN! (DefeatScreenClass atanmamis - Character Blueprint'te ata)"));
	}
}

void AToxicDepthsLastCharacter::RestartLevel()
{
	// Þu anki level'ý yeniden yükle (Tekrar Dene butonu)
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
}

void AToxicDepthsLastCharacter::SetOxygenDrainRate(float NewRate)
{
	OxygenDrainRate = NewRate;
}

void AToxicDepthsLastCharacter::ResetOxygenDrainRate()
{
	OxygenDrainRate = 1.0f;
}

//////////////////////////////////////////////////////////////////////////// Input

void AToxicDepthsLastCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AToxicDepthsLastCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AToxicDepthsLastCharacter::Look);

		// Interact (E)
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AToxicDepthsLastCharacter::Interact);

		// Use Oxygen Tank (F)
		EnhancedInputComponent->BindAction(UseTankAction, ETriggerEvent::Started, this, &AToxicDepthsLastCharacter::UseOxygenTank);

		// Use Medkit (G)
		EnhancedInputComponent->BindAction(UseMedkitAction, ETriggerEvent::Started, this, &AToxicDepthsLastCharacter::UseMedkit);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AToxicDepthsLastCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AToxicDepthsLastCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


void AToxicDepthsLastCharacter::Interact()
{
	// Menü zaten açýksa, E'ye basýnca kapat
	if (ShopMenuWidget)
	{
		CloseShopMenu();
		return;
	}
	// Kameranýn konumu ve baktýðý yön
	FVector CameraLocation = FirstPersonCameraComponent->GetComponentLocation();
	FVector CameraForward = FirstPersonCameraComponent->GetForwardVector();
	// Iþýnýn baþlangýcý (kamera) ve bitiþi (300 birim ileri)
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = CameraLocation + (CameraForward * 300.0f);
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult, TraceStart, TraceEnd, ECC_Visibility, Params);
	if (bHit && HitResult.GetActor())
	{
		// 1) Baktýðýmýz þey toplanabilir bir nesne mi?
		APickupItem* Pickup = Cast<APickupItem>(HitResult.GetActor());
		if (Pickup)
		{
			if (Pickup->PickupType == EPickupType::Gold)
			{
				Gold += Pickup->Amount;
			}
			else // Part
			{
				Parts += Pickup->Amount;
			}
			Pickup->Collect();
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					4, 2.0f, FColor::Green, TEXT("Topladin!"));
			}
			return; // iþimiz bitti
		}
		// 2) Baktýðýmýz þey satýcý mý?
		AMerchant* Merchant = Cast<AMerchant>(HitResult.GetActor());
		if (Merchant)
		{
			CurrentMerchant = Merchant;   // hangi satýcýyla konuþtuðumuzu hatýrla
			OpenShopMenu();
			return; // iþimiz bitti
		}
		// 3) Baktýðýmýz þey asansör mü?
		AElevator* Elevator = Cast<AElevator>(HitResult.GetActor());
		if (Elevator)
		{
			Elevator->DeliverPart(this);
			return;
		}
		// 4) Baktýðýmýz þey hiçbiri deðil
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				4, 2.0f, FColor::Orange, TEXT("Bu etkilesimli bir sey degil"));
		}
	}
	else
	{
		// Hiçbir þeye deðmedik
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				4, 2.0f, FColor::Orange, TEXT("Hicbir seye bakmiyorsun"));
		}
	}
}


void AToxicDepthsLastCharacter::UseOxygenTank()
{
	// Elimizde tüp var mý?
	if (OxygenTanks <= 0)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(7, 2.0f, FColor::Red, TEXT("Tup yok!"));
		}
		return;
	}

	// Oksijen zaten doluysa harcama
	if (Oxygen >= MaxOxygen)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(7, 2.0f, FColor::Orange, TEXT("Oksijen zaten dolu!"));
		}
		return;
	}

	OxygenTanks -= 1;
	Oxygen = MaxOxygen; // 60'a doldur

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(7, 2.0f, FColor::Green, TEXT("Tup kullandin! Oksijen doldu."));
	}
}

void AToxicDepthsLastCharacter::UseMedkit()
{
	// Elimizde medkit var mý?
	if (Medkits <= 0)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(8, 2.0f, FColor::Red, TEXT("Medkit yok!"));
		}
		return;
	}

	// Can zaten doluysa harcama
	if (Health >= MaxHealth)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(8, 2.0f, FColor::Orange, TEXT("Can zaten dolu!"));
		}
		return;
	}

	Medkits -= 1;
	Health = MaxHealth; // 100'e doldur

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(8, 2.0f, FColor::Green, TEXT("Medkit kullandin! Can doldu."));
	}
}


void AToxicDepthsLastCharacter::OpenShopMenu()
{
	// Zaten açýksa tekrar açma
	if (ShopMenuWidget) { return; }
	if (!ShopMenuClass) { return; }

	// Menüyü oluþtur ve ekrana koy
	ShopMenuWidget = CreateWidget<UUserWidget>(GetWorld(), ShopMenuClass);
	if (ShopMenuWidget)
	{
		ShopMenuWidget->AddToViewport();

		// Fareyi göster ve UI'a týklanabilsin
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			PC->SetShowMouseCursor(true);
			FInputModeGameAndUI InputMode;
			PC->SetInputMode(InputMode);
		}
	}
}

void AToxicDepthsLastCharacter::CloseShopMenu()
{
	if (ShopMenuWidget)
	{
		ShopMenuWidget->RemoveFromParent();
		ShopMenuWidget = nullptr;

		// Fareyi gizle, oyun kontrolüne dön
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			PC->SetShowMouseCursor(false);
			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
		}
	}
}


void AToxicDepthsLastCharacter::BuyOxygenTank()
{
	// Satýcý yoksa çýk
	if (!CurrentMerchant) { return; }

	int32 Price = CurrentMerchant->OxygenTankPrice;

	// Altýn yeterli mi?
	if (Gold >= Price)
	{
		Gold -= Price;                 // altýný öde
		OxygenTanks += 1;              // tüp sayýsýný artýr
		CurrentMerchant->OxygenTankPrice += CurrentMerchant->PriceIncrease; // fiyatý zamla

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				6, 2.0f, FColor::Green, TEXT("Oksijen tupu aldin!"));
		}
	}
	else
	{
		// Para yetmiyor
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				6, 2.0f, FColor::Red, TEXT("Yeterli altin yok!"));
		}
	}
}



void AToxicDepthsLastCharacter::BuyPart()
{
	if (!CurrentMerchant) { return; }

	int32 Price = CurrentMerchant->PartPrice;
	if (Gold >= Price)
	{
		Gold -= Price;
		Parts += 1;
		CurrentMerchant->PartPrice += CurrentMerchant->PriceIncrease;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(6, 2.0f, FColor::Green, TEXT("Parca aldin!"));
		}
	}
	else if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(6, 2.0f, FColor::Red, TEXT("Yeterli altin yok!"));
	}
}

void AToxicDepthsLastCharacter::BuyMedkit()
{
	if (!CurrentMerchant) { return; }

	int32 Price = CurrentMerchant->MedkitPrice;
	if (Gold >= Price)
	{
		Gold -= Price;
		Medkits += 1;
		CurrentMerchant->MedkitPrice += CurrentMerchant->PriceIncrease;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(6, 2.0f, FColor::Green, TEXT("Medkit aldin!"));
		}
	}
	else if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(6, 2.0f, FColor::Red, TEXT("Yeterli altin yok!"));
	}
}

void AToxicDepthsLastCharacter::BuyAmmo()
{
	if (!CurrentMerchant) { return; }

	int32 Price = CurrentMerchant->AmmoPrice;
	if (Gold >= Price)
	{
		Gold -= Price;
		Ammo += 10; // her alýmda 10 mermi
		CurrentMerchant->AmmoPrice += CurrentMerchant->PriceIncrease;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(6, 2.0f, FColor::Green, TEXT("Mermi aldin!"));
		}
	}
	else if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(6, 2.0f, FColor::Red, TEXT("Yeterli altin yok!"));
	}
}

void AToxicDepthsLastCharacter::SellPart()
{
	if (!CurrentMerchant) { return; }

	// Elimizde satacak parça var mý?
	if (Parts > 0)
	{
		Parts -= 1;
		Gold += CurrentMerchant->PartSellValue; // parça sat, altýn kazan

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(6, 2.0f, FColor::Green, TEXT("Parca sattin!"));
		}
	}
	else if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(6, 2.0f, FColor::Red, TEXT("Satacak parca yok!"));
	}
}