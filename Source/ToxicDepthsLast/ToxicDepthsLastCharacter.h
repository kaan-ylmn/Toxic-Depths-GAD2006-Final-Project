// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ToxicDepthsLastCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class UUserWidget;
class AMerchant;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)
class AToxicDepthsLastCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Interact (E) Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	/** Use Oxygen Tank (F) Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* UseTankAction;

	/** Use Medkit (G) Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* UseMedkitAction;

public:
	AToxicDepthsLastCharacter();

protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime) override;

	// Oyuncu öldüðünde çaðrýlýr
	void OnDeath();

public:
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for interact (E) input */
	void Interact();

	/** F ile oksijen tüpü kullan */
	void UseOxygenTank();

	/** G ile medkit kullan */
	void UseMedkit();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	// --- ToxicDepths survival stats ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToxicDepths|Stats")
	float Oxygen = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToxicDepths|Stats")
	float MaxOxygen = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToxicDepths|Stats")
	float Health = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToxicDepths|Stats")
	float MaxHealth = 100.f;

	// Oksijenin þu an saniyede ne kadar azaldýðý (0 = güvenli oda, 1 = normal, 2 = gaz)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToxicDepths|Stats")
	float OxygenDrainRate = 1.0f;

	// Oksijen azalma hýzýný ayarlar (trigger'lar bunu çaðýracak)
	UFUNCTION(BlueprintCallable, Category = "ToxicDepths|Stats")
	void SetOxygenDrainRate(float NewRate);

	// Normal hýza (1.0) geri döndürür
	UFUNCTION(BlueprintCallable, Category = "ToxicDepths|Stats")
	void ResetOxygenDrainRate();

	// --- ToxicDepths inventory (sadece sayaç, tam envanter yok) ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToxicDepths|Inventory")
	int32 Gold = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToxicDepths|Inventory")
	int32 Parts = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToxicDepths|Inventory")
	int32 OxygenTanks = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToxicDepths|Inventory")
	int32 Medkits = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToxicDepths|Inventory")
	int32 Ammo = 0;

	// --- ToxicDepths shop (satýcý menüsü) ---
	// Hangi menü açýlacak (editörde WBP_ShopMenu'yu buraya atayacaðýz)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToxicDepths|Shop")
	TSubclassOf<UUserWidget> ShopMenuClass;

	// Ekranda açýk duran menü (varsa)
	UPROPERTY()
	UUserWidget* ShopMenuWidget = nullptr;

	// Þu an konuþtuðumuz satýcý (menü açýkken fiyatlarý buradan okuyacaðýz)
	UPROPERTY()
	AMerchant* CurrentMerchant = nullptr;

	// Menüyü aç / kapat
	void OpenShopMenu();
	void CloseShopMenu();

	// Oksijen tüpü satýn al (menü butonu bunu çaðýracak)
	UFUNCTION(BlueprintCallable, Category = "ToxicDepths|Shop")
	void BuyOxygenTank();

	// Asansör parçasý satýn al
	UFUNCTION(BlueprintCallable, Category = "ToxicDepths|Shop")
	void BuyPart();

	// Can (medkit) satýn al
	UFUNCTION(BlueprintCallable, Category = "ToxicDepths|Shop")
	void BuyMedkit();

	// Mermi satýn al
	UFUNCTION(BlueprintCallable, Category = "ToxicDepths|Shop")
	void BuyAmmo();

	// Parça sat (altýn kazan)
	UFUNCTION(BlueprintCallable, Category = "ToxicDepths|Shop")
	void SellPart();

	// --- ToxicDepths yenilgi ekraný ---
	// Hangi widget gösterilecek (editörde WBP_DefeatScreen'i buraya atayacaðýz)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToxicDepths|UI")
	TSubclassOf<UUserWidget> DefeatScreenClass;

	// Ekranda açýk duran yenilgi ekraný (varsa)
	UPROPERTY()
	UUserWidget* DefeatScreenWidget = nullptr;

	// Oyuncu zaten öldü mü (OnDeath'in tekrar tetiklenmesini engeller)
	UPROPERTY(BlueprintReadOnly, Category = "ToxicDepths|UI")
	bool bIsDead = false;

	// Yenilgi ekranýndaki "Tekrar Dene" butonu bunu çaðýracak
	UFUNCTION(BlueprintCallable, Category = "ToxicDepths|UI")
	void RestartLevel();
};