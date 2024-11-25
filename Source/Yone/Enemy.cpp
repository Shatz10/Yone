// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "YoneGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);

	EnemyFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook"));
	EnemyFlipbook->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsAlive && CanFollow && Player)
	{
		// Move towards the player
		FVector CurrentLocation = GetActorLocation();
		FVector PlayerLocation = Player->GetActorLocation();

		FVector DirectionToPlayer = PlayerLocation - CurrentLocation;
		float DistanceToPlayer = DirectionToPlayer.Length();

		if (DistanceToPlayer >= StopDistance)
		{
			DirectionToPlayer.Normalize();
			FVector NewLocation = CurrentLocation + (DirectionToPlayer * MovementSpeed * DeltaTime);
			NewLocation.Y = 0;
			// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, NewLocation.ToString());
			SetActorLocation(NewLocation);
		}


		CurrentLocation = GetActorLocation();
		float FlipbookXScale = EnemyFlipbook->GetComponentScale().X;

		if ((PlayerLocation.X - CurrentLocation.X) >= 0.0f) // Player is on the right side of the enemy
		{
			if (FlipbookXScale < 0.0f)
			{
				EnemyFlipbook->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
			}
		}
		else // Player is on the left side of the enemy
		{
			if (FlipbookXScale > 0.0f)
			{
				EnemyFlipbook->SetWorldScale3D(FVector(-1.0f, 1.0f, 1.0f));
			}
		}
	}

}

void AEnemy::Die()
{
	if (!IsAlive) return;
	IsAlive = false;
	EnemyFlipbook->SetFlipbook(DeadFlipbook);
	EnemyFlipbook->SetTranslucentSortPriority(-5);

	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	EnemyDiedDelegate.Broadcast();
	
	UGameplayStatics::PlaySound2D(GetWorld(), DieSound);
	
	GetWorldTimerManager().SetTimer(DestoryTimer, this, &AEnemy::OnDestroyTimerTimeOut, 1, false, 10);
}

void AEnemy::OnDestroyTimerTimeOut()
{
	Destroy();
}

