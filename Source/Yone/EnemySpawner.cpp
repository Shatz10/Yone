// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

#include "Enemy.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	AGameModeBase *GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (GameMode)
	{
		MyGameMode = Cast<AYoneGameModeBase>(GameMode);
		check(MyGameMode);
	}

	AActor* PlayerActor = UGameplayStatics::GetActorOfClass(GetWorld(), AYonePawn::StaticClass());
	if (PlayerActor)
	{
		Player = Cast<AYonePawn>(PlayerActor);
		// Player->PlayerDiedDelegate.AddDynamic(this, &AEnemySpawner::OnPlayerDied);
	}

	StartSpawning();
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AEnemySpawner::StartSpawning()
{
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AEnemySpawner::OnSpawnTimerTimeout, SpawnTime, true, SpawnTime);
}

void AEnemySpawner::StopSpawning()
{
	GetWorldTimerManager().ClearTimer(SpawnTimer);
}

void AEnemySpawner::OnSpawnTimerTimeout()
{
	SpawnEnemy();
}

void AEnemySpawner::SpawnEnemy()
{
	// Spawn the enemy
	FVector2D RandomPosition = FVector2D(FMath::VRand());
	RandomPosition.Normalize();
	RandomPosition *= SpawnDistance;

	FVector EnemyLocation = GetActorLocation() + FVector(RandomPosition.X, 0.0f, RandomPosition.Y);

	AEnemy *Enemy = GetWorld()->SpawnActor<AEnemy>(EnemyActorToSpawn, EnemyLocation, FRotator::ZeroRotator);
	SetupEnemy(Enemy);

	// Increase The Difficulty
	TotalEnemyCount += 1;
	if ((TotalEnemyCount % DifficultySpawnInterval) == 0)
	{
		if (SpawnTime > SpawnTimeMinimumLimit)
		{
			SpawnTime -= DecreaseSpawnTimerByEveryInterval;
			if (SpawnTime < SpawnTimeMinimumLimit)
			{
				SpawnTime = SpawnTimeMinimumLimit;
			}

			StopSpawning();
			StartSpawning();
		}
	}
}

void AEnemySpawner::SetupEnemy(AEnemy* Enemy)
{
	if (Enemy)
	{
		Enemy->Player = Player;
		Enemy->CanFollow = true;
		Enemy->EnemyDiedDelegate.AddDynamic(this, &AEnemySpawner::OnEnemyDied);
	}
}

void AEnemySpawner::OnEnemyDied()
{
	int ScoreToAdd = 10;
	MyGameMode->AddScore(ScoreToAdd);
}

void AEnemySpawner::OnPlayerDied()
{
	StopSpawning();

	TArray<AActor*> EnemyArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), EnemyArray);
	for (AActor *EnemyActor : EnemyArray)
	{
		AEnemy* Enemy = Cast<AEnemy>(EnemyActor);
		if (Enemy && Enemy->IsAlive)
		{
			Enemy->CanFollow = false;
		}
	}

	MyGameMode->RestartGame();
}


