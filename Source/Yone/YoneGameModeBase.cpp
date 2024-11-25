// Fill out your copyright notice in the Description page of Project Settings.


#include "YoneGameModeBase.h"

#include "Kismet/GameplayStatics.h"

AYoneGameModeBase::AYoneGameModeBase()
{
}

void AYoneGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	SetScore(0);
}

void AYoneGameModeBase::SetScore(int NewScore)
{
	if (NewScore >= 0)
	{
		Score = NewScore;
		ScoreChangedDelegate.Broadcast(Score);
	}
}

void AYoneGameModeBase::AddScore(int AmountToAdd)
{
	int NewScore = Score + AmountToAdd;
	SetScore(NewScore);
}

void AYoneGameModeBase::RestartGame()
{
	GetWorldTimerManager().SetTimer(RestartGameTimer, this, &AYoneGameModeBase::OnRestartGameTimerTimeout, 1.0f, false, TimeBeforeRestart);
}

void AYoneGameModeBase::OnRestartGameTimerTimeout()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainLevel"));
}
