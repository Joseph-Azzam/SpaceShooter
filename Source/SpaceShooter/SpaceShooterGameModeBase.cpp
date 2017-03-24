// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceShooter.h"
#include "SpaceShooterGameModeBase.h"
#include "EnemyController.h"
#include "GameWidget.h"

ASpaceShooterGameModeBase::ASpaceShooterGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	GameTimer = 0;
	EnemyTimer = 0;
}

void ASpaceShooterGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	ChangeMenuWidget(StartingWidgetClass);

	((UGameWidget*)CurrentWidget)->Load();
}

void ASpaceShooterGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GameTimer += DeltaTime;
	EnemyTimer -= DeltaTime;

	if (EnemyTimer <= 0.0f)
	{
		float difficultyPercentage = FMath::Min(GameTimer / TIME_TO_MINIMUM_INTERVAL, 1.0f);
		EnemyTimer = MAXIMUM_INTERVAL - (MAXIMUM_INTERVAL - MINIMUM_INTERVAL) * difficultyPercentage;

		UWorld* World = GetWorld();
		if (World)
		{
			FVector Location = FVector(600.0f, FMath::RandRange(-800.0f, 800.0f), 70.0f);
			World->SpawnActor<AEnemyController>(EnemyBlueprint, Location, FRotator::ZeroRotator);
		}
	}
}

void ASpaceShooterGameModeBase::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);

		if (CurrentWidget != nullptr)

		{
			CurrentWidget->AddToViewport();
		}
	}
}

void ASpaceShooterGameModeBase::IncrementScore()
{
	Score += 100;
	((UGameWidget*)CurrentWidget)->SetScore(Score);
}

void ASpaceShooterGameModeBase::OnGameOver()
{
	((UGameWidget*)CurrentWidget)->OnGameOver(Score);
}