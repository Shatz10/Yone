// Fill out your copyright notice in the Description page of Project Settings.


#include "YonePawn.h"

#include "Bullet.h"
#include "InputMappingContext.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AYonePawn::AYonePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule);

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook"));
	FlipbookComponent->SetupAttachment(RootComponent);

	GunParent = CreateDefaultSubobject<USceneComponent>(TEXT("GunParent"));
	GunParent->SetupAttachment(RootComponent);

	GunSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("GunSprite"));
	GunSprite->SetupAttachment(GunParent);

	BulletSpawnPosition = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawnPosition"));
	BulletSpawnPosition->SetupAttachment(GunSprite);
}

// Called when the game starts or when spawned
void AYonePawn::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		PlayerController->SetShowMouseCursor(true);
		
		UEnhancedInputLocalPlayerSubsystem* Subsystem  = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}	
	}
}

// Called every frame
void AYonePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CanMove)
	{
		if (MoveDirection.Length() > 0)
		{
			if (MoveDirection.Length() > 1)
			{
				MoveDirection.Normalize();
			}

			FVector2D MoveDistance = MoveDirection * MoveSpeed * DeltaTime;
			FVector CurrentLocation = GetActorLocation();

			FVector NewLocation = CurrentLocation + FVector(MoveDistance.X, 0, 0);

			if (!IsInMapBoundsHorizontal(NewLocation.X))
			{
				NewLocation -= FVector(MoveDistance.X, 0, 0);
			}
			
			NewLocation += FVector(0, 0, MoveDistance.Y);
			if (!IsInMapBoundsVertical(NewLocation.Z))
			{
				NewLocation -= FVector(0, 0, MoveDistance.Y);
			}
			// NewLocation += FVector(MoveDistance.X, 0, 0);

			SetActorLocation(NewLocation);
		}
	}

	// Rotate the gun
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		FVector MouseWorldLocation, MouseWorldDirection;
		PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);

		FVector CurrentLocation = GetActorLocation();
		FVector Start = FVector(CurrentLocation.X, 0.0f, CurrentLocation.Z);
		FVector Target = FVector(MouseWorldLocation.X, 0.0f, MouseWorldLocation.Z);
		FRotator GunParentRotator = UKismetMathLibrary::FindLookAtRotation(Start, Target);

		GunParent->SetRelativeRotation(GunParentRotator);
	}
}

// Called to bind functionality to input
void AYonePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AYonePawn::MoveTriggered);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AYonePawn::MoveCompleted);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &AYonePawn::MoveCompleted);

		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AYonePawn::Shoot);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AYonePawn::Shoot);
	}
}

void AYonePawn::MoveTriggered(const FInputActionValue& Value)
{
	FVector2d MoveValue = Value.Get<FVector2d>();

	if (CanMove)
	{
		MoveDirection = MoveValue;
		FlipbookComponent->SetFlipbook(RunFlipbook);

		FVector Scale = FlipbookComponent->GetComponentScale();
		if(MoveDirection.X < 0)
		{
			if(Scale.X > 0)
			{
				FlipbookComponent->SetWorldScale3D(FVector(-1.0f, 1.0f, 1.0f));
			}
		}

		if(MoveDirection.X > 0)
		{
			if(Scale.X < 0)
			{
				FlipbookComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
			}
		}
	}
	// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, MoveValue.ToString());
}

void AYonePawn::MoveCompleted(const FInputActionValue& Value)
{
	FVector2d MoveValue = Value.Get<FVector2d>();
	MoveDirection = MoveValue = FVector2D::ZeroVector;
	FlipbookComponent->SetFlipbook(IdleFlipbook);
	// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("MoveCompleted"));
}

void AYonePawn::Shoot(const FInputActionValue& Value)
{
	if (CanShoot)
	{
		CanShoot = false;

		ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletActor, BulletSpawnPosition->GetComponentLocation(), FRotator(0, 0, 0));
		check(Bullet);

		APlayerController* PlayerController = Cast<APlayerController>(Controller);
		check(PlayerController);
	
		FVector MouseWorldLocation, MouseWorldDirection;
		PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);

		FVector CurrentLocation = GetActorLocation();
		FVector2D BulletDirection = FVector2D(MouseWorldLocation.X - CurrentLocation.X, MouseWorldLocation.Z - CurrentLocation.Z);
		BulletDirection.Normalize();
		
		Bullet->Launch(BulletDirection, 300.0f);

		// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("Shoot" + BulletDirection.ToString()));

		GetWorldTimerManager().SetTimer(TimerHandle, this, &AYonePawn::OnShootCooldownTimerTimeOut, 1, false, ShootCD);
	}
	
	// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("Shoot"));
}

bool AYonePawn::IsInMapBoundsHorizontal(float XPos)
{
	bool Result = true;
	Result = (XPos > HorizontalLimits.X && XPos < HorizontalLimits.Y);
	return Result;
}

bool AYonePawn::IsInMapBoundsVertical(float ZPos)
{
	bool Result = true;
	Result = (ZPos > VerticalLimits.X && ZPos < VerticalLimits.Y);
	return Result;
}

void AYonePawn::OnShootCooldownTimerTimeOut()
{
	CanShoot = true;	
}

