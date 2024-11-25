// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "PaperSpriteComponent.h"
#include "Engine/TimerHandle.h"

#include "Bullet.generated.h"

UCLASS()
class YONE_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* Sphere;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* Sprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D MoveDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsLaunched;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsDisabled;

	FTimerHandle DeleteTimerHandle;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Launch(FVector2d Direction, float Speed);

	void OnDeleteTimerTimeOut();

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void DestoyBullet();
};
