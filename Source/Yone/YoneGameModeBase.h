// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "YoneGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScoreChangedDelegate, int, NewScore);

/**
 * 
 */
UCLASS()
class YONE_API AYoneGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AYoneGameModeBase();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int Score = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeBeforeRestart = 0.3f;

	UPROPERTY(BlueprintAssignable)
	FScoreChangedDelegate ScoreChangedDelegate;

	FTimerHandle RestartGameTimer;
	
	virtual void BeginPlay() override;
	void SetScore(int NewScore);
	void AddScore(int AmountToAdd);

	void RestartGame();
	void OnRestartGameTimerTimeout();
};
