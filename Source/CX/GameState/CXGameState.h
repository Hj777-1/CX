// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CXGameState.generated.h"

class ACXPlayerState;

UENUM(BlueprintType)
enum class EGameState : uint8
{
	Waiting,
	Playing,
	Finished
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnRemainingTimeChanged, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChanged, EGameState);

UCLASS()
class CX_API ACXGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetGameState(EGameState NewGameState);

	void SetRemainingTime(int32 NewRemainingTime);

	EGameState GetCurrentGameState() const { return CurrentGameState; }
	int32 GetRemainingTime() const { return RemainingTime; }

	FOnRemainingTimeChanged OnRemainingTimeChanged;
	FOnGameStateChanged OnGameStateChanged;

protected:
	UFUNCTION()
	void OnRep_CurrentGameState();

	UFUNCTION()
	void OnRep_RemainingTime();

	UPROPERTY(ReplicatedUsing = OnRep_CurrentGameState)
	EGameState CurrentGameState = EGameState::Waiting;

	UPROPERTY(ReplicatedUsing = OnRep_RemainingTime)
	int32 RemainingTime = 0;


public:
	UPROPERTY(Replicated)
	TArray<TObjectPtr<ACXPlayerState>> Winners;
};