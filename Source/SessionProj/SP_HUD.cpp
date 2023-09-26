// Fill out your copyright notice in the Description page of Project Settings.


#include "SP_HUD.h"

void ASP_HUD::UpdateFoundServers(TArray<FOnlineSessionSearchResult> SearchResults)
{
	TArray<FServerDetails> NewServerDetailsArray;

	for (int i = 0; i < SearchResults.Num(); i++)
	{
		FServerDetails NewServerDetails;
		NewServerDetails.ServerIndex = i;
		NewServerDetails.Name = SearchResults[i].Session.OwningUserName;
		NewServerDetails.MaxSpaces = SearchResults[i].Session.SessionSettings.NumPublicConnections;
		NewServerDetails.OpenSpaces = SearchResults[i].Session.NumOpenPublicConnections;

		NewServerDetailsArray.Add(NewServerDetails);
	}

	BPUpdateFoundServers(NewServerDetailsArray);
}