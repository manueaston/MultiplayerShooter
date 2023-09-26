// Fill out your copyright notice in the Description page of Project Settings.


#include "SP_GameState.h"

#include "SP_PlayerState.h"
#include "SP_GameMode.h"
#include "Net/UnrealNetwork.h"

void ASP_GameState::SetIsTie(bool IsTie)
{
	GameTied = IsTie;
}

bool ASP_GameState::GetIsTie()
{
	return GameTied;
}

void ASP_GameState::SetWinner(class ASP_PlayerState* _Winner)
{
	WinningPlayer = _Winner;
}

ASP_PlayerState* ASP_GameState::GetWinner()
{
	return WinningPlayer;
}

float ASP_GameState::GetCountdown()
{
	return Countdown;
}

void ASP_GameState::StartCountdown()
{
	GetWorldTimerManager().SetTimer(GameTimerHandle, this, &ASP_GameState::DecreaseCountdown, 1.0f, true, 0.0f);
}

void ASP_GameState::DecreaseCountdown()
{
	Countdown--;

	if (Countdown >= 0.0f)
	{
		if (ASP_GameMode* GameMode = Cast<ASP_GameMode>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->CheckForWinner();
		}
	}
}

void ASP_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASP_GameState, WinningPlayer);
	DOREPLIFETIME(ASP_GameState, Countdown);
	DOREPLIFETIME(ASP_GameState, GameTied);
}
