// Fill out your copyright notice in the Description page of Project Settings.


#include "SP_GameMode.h"

#include "SP_GameState.h"
#include "SP_PlayerState.h"

void ASP_GameMode::RespawnPlayer(APlayerController* _PlayerToRespawn)
{
	//Respawn only if a match is in progress
	if (IsMatchInProgress())
	{
		RestartPlayer(_PlayerToRespawn);
	}
}

void ASP_GameMode::CheckForWinner()
{
	if (ASP_GameState* SPGameState = GetGameState<ASP_GameState>())
	{
		int highestScore = 0;
		ASP_PlayerState* winningPlayer = nullptr;

		for (auto iter : SPGameState->PlayerArray)
		{
			if (ASP_PlayerState* SPPlayerState = Cast<ASP_PlayerState>(iter))
			{
				int playerEliminations = SPPlayerState->GetEliminations();

				if (playerEliminations == highestScore) // Tie
				{
					SPGameState->SetIsTie(true);
					winningPlayer = SPPlayerState; // Set so HUD will know game is over
				}
				else if (playerEliminations > highestScore)
				{
					highestScore = playerEliminations;
					winningPlayer = SPPlayerState;
					SPGameState->SetIsTie(false);
				}
			}
		}

		if (highestScore >= 5 || SPGameState->GetCountdown() <= 0.0f)
		{
			//Set the winner
			SPGameState->SetWinner(winningPlayer);

			//A player has reached max eliminations, end game
			EndMatch();
		}
	}
}

void ASP_GameMode::HandleMatchHasEnded()
{
	GetWorldTimerManager().SetTimer(MatchRestartDelayTimer, this, &AGameMode::RestartGame, 5.0f, false);
}