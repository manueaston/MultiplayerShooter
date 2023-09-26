// Fill out your copyright notice in the Description page of Project Settings.


#include "SP_Pickup_Ammo.h"
#include "SessionProjCharacter.h"

void ASP_Pickup_Ammo::OnPickedUp(class ASessionProjCharacter* Character)
{
	Character->GiveAmmo(AmmoToGive);
}