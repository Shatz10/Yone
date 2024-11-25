// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YoneGameModeBase.h"
#include "YonePawn.h"
#include "Enemy.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class YONE_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemy> EnemyActorToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnDistance = 400.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int TotalEnemyCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DifficultySpawnInterval = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnTimeMinimumLimit = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DecreaseSpawnTimerByEveryInterval = 0.05f;
	AYonePawn* Player;

	FTimerHandle SpawnTimer;

	AYoneGameModeBase* MyGameMode;

	AEnemySpawner();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void OnSpawnTimerTimeout();
	void StartSpawning();
	void StopSpawning();
	void SpawnEnemy();

	void SetupEnemy(AEnemy* Enemy);

	UFUNCTION()
	void OnEnemyDied();

	UFUNCTION()
	void OnPlayerDied();
};
