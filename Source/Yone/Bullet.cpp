// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Enemy.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);

	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(RootComponent);

	MoveDirection = FVector2D(1, 0); 
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OverlapBegin);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLaunched)
	{
		FVector2d Distance = MoveDirection * MoveSpeed * DeltaTime;
		FVector CurrentLocation = GetActorLocation();
		FVector NewLocation = CurrentLocation + FVector(Distance.X,  0.0f, Distance.Y);
		SetActorLocation(NewLocation);
	}
}

void ABullet::Launch(FVector2d Direction, float Speed)
{
	if (IsLaunched) return;
	IsLaunched = true;
	MoveDirection = Direction;
	MoveSpeed = Speed;

	float DeleteTime = 10.0f;
	GetWorldTimerManager().SetTimer(DeleteTimerHandle, this, &ABullet::OnDeleteTimerTimeOut, 1, false, DeleteTime);
}

void ABullet::OnDeleteTimerTimeOut()
{
	Destroy();
}

void ABullet::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy && Enemy->IsAlive)
	{
		DestoyBullet();
		Enemy->Die();
	}
}

void ABullet::DestoyBullet()
{
	if (IsDisabled)
	{
		return;
	}
	IsDisabled = true;

	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Sprite->DestroyComponent();
}

