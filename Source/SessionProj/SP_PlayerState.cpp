// Fill out your copyright notice in the Description page of Project Settings.


#include "SP_PlayerState.h"

#include "Net/UnrealNetwork.h"
#include "SP_GameMode.h"

void ASP_PlayerState::GiveElimination()
{
	if (ASP_GameMode* GameMode = Cast<ASP_GameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (GameMode->IsMatchInProgress())
		{
			Eliminations++;

			GameMode->CheckForWinner();
		}
	}
}

int ASP_PlayerState::GetEliminations()
{
	return Eliminations;
}

void ASP_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASP_PlayerState, Eliminations);
}