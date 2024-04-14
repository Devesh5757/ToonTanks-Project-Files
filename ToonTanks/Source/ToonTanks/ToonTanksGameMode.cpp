// Fill out your copyright notice in the Description page of Project Settings.

#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"
#include "TimerManager.h"

void AToonTanksGameMode::ActorDied(AActor *DeadActor)
{
    if (DeadActor == Tank)
    {
        Tank->HandleDestruction();
        if (ToonTanksPlayerController)
        {
            // Tank->DisableInput(Tank->GetTankPlayerController());
            // Tank->GetTankPlayerController()->bShowMouseCursor = false;
            ToonTanksPlayerController->SetPlayerEnableState(false);
        }
        GameOver(false);
        // To restart the game after 5 sec when player lose
        //  GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &AToonTanksGameMode::RestartGame, 5.f, false);

        AToonTanksPlayerController *PlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
        if (PlayerController)
        {
            PlayerController->bCanRestartGame = true;
        }
    }
    else if (ATower *DestroyedTower = Cast<ATower>(DeadActor))
    {
        DestroyedTower->HandleDestruction();
        --TargetTower;

        if (TargetTower == 0)
        {
            GameOver(true);
            // To Restart the game after 5 sec when player wins
            //  GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &AToonTanksGameMode::RestartGame, 5.f, false);

            AToonTanksPlayerController *PlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
            if (PlayerController)
            {
                PlayerController->bCanRestartGame = true;
            }
        }
    }
}

void AToonTanksGameMode::BeginPlay()
{
    Super::BeginPlay();

    HandleGameStart();
}

void AToonTanksGameMode::HandleGameStart()
{
    TargetTower = GetTargetTowerCount();
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    StartGame();

    if (ToonTanksPlayerController)
    {
        ToonTanksPlayerController->SetPlayerEnableState(false);

        FTimerHandle PlayerEnableTimerHandle;
        FTimerDelegate InputDelegate = FTimerDelegate::CreateUObject(
            ToonTanksPlayerController,
            &AToonTanksPlayerController::SetPlayerEnableState,
            true);
        GetWorldTimerManager().SetTimer(
            PlayerEnableTimerHandle,
            InputDelegate,
            StartDelay,
            false);
    }
}

int32 AToonTanksGameMode::GetTargetTowerCount()
{
    TArray<AActor *> Towers;
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);

    return Towers.Num();
}

// void AToonTanksGameMode::RestartGame()
// {
//     FString Main = UGameplayStatics::GetCurrentLevelName(GetWorld());
//     UGameplayStatics::OpenLevel(GetWorld(), FName(*Main));
// }
