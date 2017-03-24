// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceShooter.h"
#include "ShipController.h"
#include "BulletController.h"
#include "EnemyController.h"
#include "SpaceShooterGameModeBase.h"

// Sets default values
AShipController::AShipController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	CollisionBox->bGenerateOverlapEvents = true;
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AShipController::OnOverlap);
	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void AShipController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShipController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!CurrentVelocity.IsZero())
	{
		FVector NewLocation = GetActorLocation() + Speed * CurrentVelocity * DeltaTime;
		SetActorLocation(NewLocation);
	}

}

// Called to bind functionality to input
void AShipController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Bind Input to Functions
	InputComponent->BindAxis("MoveForward", this, &AShipController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AShipController::MoveRight);
	InputComponent->BindAction("Shoot", IE_Pressed, this, &AShipController::OnShoot);
	InputComponent->BindAction("Restart", IE_Pressed, this, &AShipController::OnRestart).bExecuteWhenPaused = true;

}

void AShipController::MoveForward(float AxisValue)
{
	CurrentVelocity.X = AxisValue*100.0f; //100.0f is equal to one unit, so we move one unit at a time
}

void AShipController::MoveRight(float AxisValue)
{
	CurrentVelocity.Y = AxisValue*100.0f;
}

void AShipController::OnShoot()
{
	
	UWorld* World = GetWorld();

	if (World)
	{
		FVector Location = GetActorLocation();
		Location.X += 30;
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Shoot!"));
		World->SpawnActor<ABulletController>(BulletBlueprint, Location, FRotator::ZeroRotator);
	}
}

void AShipController::OnRestart()
{
	if (Died)
	{
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	}
}

void AShipController::OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AEnemyController::StaticClass()))
	{
		Died = true;

		this->SetActorHiddenInGame(true);
		((ASpaceShooterGameModeBase*)GetWorld()->GetAuthGameMode())->OnGameOver();
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

