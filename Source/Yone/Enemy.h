// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "YonePawn.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyDiedDelegate);

UCLASS()
class YONE_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* EnemyFlipbook;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* DeadFlipbook;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AYonePawn* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAlive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanFollow = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StopDistance = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle DestoryTimer;

	FEnemyDiedDelegate EnemyDiedDelegate;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Die();
	void OnDestroyTimerTimeOut();
};