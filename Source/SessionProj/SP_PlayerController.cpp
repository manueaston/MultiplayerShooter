// Fill out your copyright notice in the Description page of Project Settings.


#include "SP_PlayerController.h"

#include "SP_GameMode.h"

//Respawns after a delay
void ASP_PlayerController::RespawnAfterDelay()
{
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &ASP_PlayerController::TryRespawn, 5.0f, false);
}

void ASP_PlayerController::TryRespawn()
{
	if (ASP_GameMode* GameMode = Cast<ASP_GameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->RespawnPlayer(this);
	}
}