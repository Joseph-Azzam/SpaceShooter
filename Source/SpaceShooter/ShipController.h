// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "ShipController.generated.h"

UCLASS()
class SPACESHOOTER_API AShipController : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShipController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
		UShapeComponent* CollisionBox;
	UPROPERTY(EditAnywhere)
		float Speed = 10.0f;
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class ABulletController> BulletBlueprint;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void OnShoot();
	void OnRestart();

	FVector CurrentVelocity;

	bool Died;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	
};
