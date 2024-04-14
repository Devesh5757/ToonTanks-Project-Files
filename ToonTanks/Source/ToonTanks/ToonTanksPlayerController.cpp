// Fill out your copyright notice in the Description page of Project Settings.

#include "ToonTanksPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

void AToonTanksPlayerController::SetPlayerEnableState(bool bPlayerEnabled)
{
    if (bPlayerEnabled)
    {
        GetPawn()->EnableInput(this);
    }
    else
    {
        GetPawn()->DisableInput(this);
    }
    bShowMouseCursor = bPlayerEnabled;
}

void AToonTanksPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("Restart", IE_Pressed, this, &AToonTanksPlayerController::RestartGame);
    InputComponent->BindAction("Exit", IE_Pressed, this, &AToonTanksPlayerController::ExitGame);
}

void AToonTanksPlayerController::RestartGame()
{
    if (bCanRestartGame)
    {
        FString Main = UGameplayStatics::GetCurrentLevelName(GetWorld());
        UGameplayStatics::OpenLevel(GetWorld(), FName(*Main));
    }
}

void AToonTanksPlayerController::ExitGame()
{
    if (bCanRestartGame)
    {
        // Close the game
        GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
    }
}
