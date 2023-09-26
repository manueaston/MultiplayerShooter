// Fill out your copyright notice in the Description page of Project Settings.


#include "SP_GameInstance.h"
#include "SP_HUD.h"

USP_GameInstance::USP_GameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &USP_GameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &USP_GameInstance::OnStartSessionComplete);
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &USP_GameInstance::OnFindSessionsComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &USP_GameInstance::OnJoinSessionComplete);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &USP_GameInstance::OnDestroySessionComplete);
}

bool USP_GameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int MaxNumPlayers)
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			SessionSettings = MakeShareable(new FOnlineSessionSettings());

			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;
			SessionSettings->bUseLobbiesIfAvailable = true;

			SessionSettings->Set(SETTING_MAPNAME, FString("ThirdPersonMap"), EOnlineDataAdvertisementType::ViaOnlineService);
			SessionSettings->Set(SEARCH_KEYWORDS, FString("Custom"), EOnlineDataAdvertisementType::ViaOnlineService);

			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			// Required for levelOptions in StartSessionComplete
			bLanMatch = bIsLAN;

			return Sessions->CreateSession(*UserId, SessionName, *SessionSettings);
		}
	}

	return false;
}

void USP_GameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);

			if (bWasSuccessful)
			{
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

				Sessions->StartSession(SessionName);
			}
		}
	}
}

void USP_GameInstance::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);

			if (bWasSuccessful)
			{
				FString levelOptions = "listen";

				if (bLanMatch)
				{
					levelOptions.Append("?bIsLanMatch=1");
				}

				UGameplayStatics::OpenLevel(GetWorld(), "ThirdPersonMap", true, levelOptions);
			}
		}
	}
}

void USP_GameInstance::FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence)
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());

			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = 20;
			SessionSearch->PingBucketSize = 50;

			SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);

			SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);

			SessionSearch->QuerySettings.Set(SEARCH_KEYWORDS, FString("Custom"), EOnlineComparisonOp::Equals);

			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();

			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			Sessions->FindSessions(*UserId, SearchSettingsRef);
		}
	}
}

void USP_GameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num()));

			for (int i = 0; i < SessionSearch->SearchResults.Num(); i++)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Session Number: %d | Session Name: %s"), 
					i + 1, *(SessionSearch->SearchResults[i].Session.OwningUserName)));
			}
		}

		APlayerController* const PlayerController = GetFirstLocalPlayerController();

		if (PlayerController)
		{
			if (ASP_HUD* SPHUD = Cast<ASP_HUD>(PlayerController->GetHUD()))
			{
				SPHUD->UpdateFoundServers(SessionSearch->SearchResults);
			}
		}
	}
}

bool USP_GameInstance::JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult)
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

			return Sessions->JoinSession(*UserId, SessionName, SearchResult);
		}
	}

	return false;
}

void USP_GameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

			APlayerController* const PlayerController = GetFirstLocalPlayerController();

			FString TravelURL;

			if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
			{
				PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
			}
		}


	}
}

void USP_GameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);

			if (bWasSuccessful)
			{
				UGameplayStatics::OpenLevel(GetWorld(), "MainMenu", true, "listen");
			}
		}
	}
}

void USP_GameInstance::StartOnlineGame(bool IsLAN)
{
	ULocalPlayer* const Player = GetFirstGamePlayer();

	HostSession(Player->GetPreferredUniqueNetId().GetUniqueNetId(), NAME_GameSession, IsLAN, true, 8);
}

//*********//
// Change so that LAN can be selected
//********//

void USP_GameInstance::FindOnlineGames(bool IsLAN)
{
	ULocalPlayer* const Player = GetFirstGamePlayer();

	FindSessions(Player->GetPreferredUniqueNetId().GetUniqueNetId(), IsLAN, true);
}

void USP_GameInstance::JoinOnlineGame(int ServerIndex)
{
	ULocalPlayer* const Player = GetFirstGamePlayer();

	FOnlineSessionSearchResult SearchResult;

	// If array is not empty, can go throught it
	if (SessionSearch->SearchResults.Num() > ServerIndex)
	{
		/*if (SessionSearch->SearchResults[ServerIndex].Session.OwningUserId != Player->GetPreferredUniqueNetId() || CanJoinSelf)
		{*/
			SearchResult = SessionSearch->SearchResults[ServerIndex];

			JoinSession(Player->GetPreferredUniqueNetId().GetUniqueNetId(), NAME_GameSession, SearchResult);
		//}
	}
}