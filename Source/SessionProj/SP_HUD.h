// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SessionProj.h"
#include "GameFramework/HUD.h"
#include "SP_HUD.generated.h"

USTRUCT(BlueprintType)
struct FServerDetails
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
		FString Name = "";
	UPROPERTY(BlueprintReadOnly)
		int OpenSpaces = 0;
	UPROPERTY(BlueprintReadOnly)
		int MaxSpaces = 0;
	UPROPERTY(BlueprintReadOnly)
		int ServerIndex = 0;
};

/**
 * 
 */
UCLASS()
class SESSIONPROJ_API ASP_HUD : public AHUD
{
	GENERATED_BODY()
	
public:

	void UpdateFoundServers(TArray<FOnlineSessionSearchResult> SearchResults);

	UFUNCTION(BlueprintImplementableEvent)
		void BPUpdateFoundServers(const TArray<FServerDetails>& ServerDetails);
};
