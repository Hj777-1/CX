// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/CXGameState.h"
#include "Net/UnrealNetwork.h"

void ACXGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentGameState);
	DOREPLIFETIME(ThisClass, RemainingTime);
	DOREPLIFETIME(ThisClass, Winners);
}

void ACXGameState::SetGameState(EGameState NewGameState)
{
	if (!HasAuthority())
	{
		return;
	}
	CurrentGameState = NewGameState;
	OnRep_CurrentGameState();   
}

void ACXGameState::SetRemainingTime(int32 NewRemainingTime)
{
	if (!HasAuthority())
	{
		return;
	}
	RemainingTime = FMath::Max(0, NewRemainingTime);
	OnRep_RemainingTime();
}

void ACXGameState::OnRep_CurrentGameState()
{
	OnGameStateChanged.Broadcast(CurrentGameState);
}

void ACXGameState::OnRep_RemainingTime()
{
	UE_LOG(LogTemp, Warning, TEXT("[OnRep] remaining=%d authority=%d"), RemainingTime, HasAuthority());
	OnRemainingTimeChanged.Broadcast(RemainingTime);
}
