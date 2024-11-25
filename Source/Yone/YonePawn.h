// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bullet.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "InputActionValue.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/Controller.h"
#include "Engine/TimerHandle.h"
#include "Sound/SoundBase.h"

#include "YonePawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDiedDelegate);

UCLASS()
class YONE_API AYonePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AYonePawn();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* Capsule;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* FlipbookComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* GunParent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* GunSprite;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* BulletSpawnPosition;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputMappingContext* MappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* ShootAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperFlipbook* IdleFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperFlipbook* RunFlipbook;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* BulletShootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* DieSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D HorizontalLimits;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D VerticalLimits;
	
	UPROPERTY(BlueprintReadWrite)
	FVector2D MoveDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanMove = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanShoot = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsAlive = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShootCD = 0.3f;

	FPlayerDiedDelegate PlayerDiedDelegate;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABullet> BulletActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BulletMoveSpeed = 300;
	
	FTimerHandle TimerHandle;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveTriggered(const FInputActionValue& Value);
	void MoveCompleted(const FInputActionValue& Value);

	void Shoot(const FInputActionValue& Value);

	bool IsInMapBoundsHorizontal(float XPos);
	bool IsInMapBoundsVertical(float ZPos);

	void OnShootCooldownTimerTimeOut();

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
