// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SP_GameState.generated.h"

/**
 * 
 */
UCLASS()
class SESSIONPROJ_API ASP_GameState : public AGameState
{
	GENERATED_BODY()
	
public:

	void SetIsTie(bool IsTie);

	UFUNCTION(BlueprintPure)
		bool GetIsTie();

	void SetWinner(class ASP_PlayerState* _Winner);

	UFUNCTION(BlueprintPure)
		ASP_PlayerState* GetWinner();

	UFUNCTION(BlueprintPure)
		float GetCountdown();

	UFUNCTION(BlueprintCallable)
		void StartCountdown();

	void DecreaseCountdown();

protected:

	UPROPERTY(Replicated)
		ASP_PlayerState* WinningPlayer = nullptr;

	UPROPERTY(Replicated)
		float Countdown = 120.0f;
	FTimerHandle GameTimerHandle;

	UPROPERTY(Replicated)
		bool GameTied = false;

};
