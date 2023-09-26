// Fill out your copyright notice in the Description page of Project Settings.


#include "SP_Pickup_Health.h"
#include "SessionProjCharacter.h"

void ASP_Pickup_Health::OnPickedUp(ASessionProjCharacter* Character)
{
	Character->GiveHealth(HealthToGive);
}