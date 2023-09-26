// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SessionProj.h"
#include "Engine/GameInstance.h"
#include "SP_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SESSIONPROJ_API USP_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	USP_GameInstance(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Network|Test")
		void StartOnlineGame(bool IsLAN);

	UFUNCTION(BlueprintCallable, Category = "Network|Test")
		void FindOnlineGames(bool IsLAN);

	UFUNCTION(BlueprintCallable, Category = "Network|Test")
		void JoinOnlineGame(int ServerIndex);

private:
	// Function Delegates
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	// Delegate Handles
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	// Functions
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int MaxNumPlayers);
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);
	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	TSharedPtr<FOnlineSessionSettings> SessionSettings;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	bool bLanMatch = false;
};
