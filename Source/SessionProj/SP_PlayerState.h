// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SP_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SESSIONPROJ_API ASP_PlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:

	//The current amount of eliminations this player has
	UPROPERTY(Replicated)
		int Eliminations = 0;

public:

	//Adds an elimination
	void GiveElimination();

	//Returns the amount of eliminations
	UFUNCTION(BlueprintPure)
		int GetEliminations();

};
