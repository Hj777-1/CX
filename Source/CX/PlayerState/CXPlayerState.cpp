// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/CXPlayerState.h"
#include "Net/UnrealNetwork.h"

static int32 GMyTestCoin = 0;

static FAutoConsoleVariableRef CVarAddCoinServer(
	TEXT("cheat,addcoin,server"),
	GMyTestCoin,
	TEXT("add coin"),
	FConsoleVariableDelegate::CreateLambda([](IConsoleVariable* CVar)
		{
			if (CVar)
			{
				UE_LOG(LogTemp, Warning, TEXT("콘솔 변수 변경됨! 현재 값: %d"), CVar->GetInt());
				UE_LOG(LogTemp, Warning, TEXT("콘솔 변수 변경됨! GMyTestCoin 값: %d"), GMyTestCoin);
				
				if (GWorld) 
				{
					if(GWorld->GetFirstPlayerController())
					{
						GWorld->GetFirstPlayerController()->GetPlayerState<ACXPlayerState>()->CTSAddCoins(GMyTestCoin);
					}
				}
				GMyTestCoin = 0;
			}
		}),
		ECVF_Cheat
	);

static FAutoConsoleVariableRef CVarAddCoinClient(
	TEXT("cheat,addcoin,client"),
	GMyTestCoin,
	TEXT("add coin test for client only"),
	FConsoleVariableDelegate::CreateLambda([](IConsoleVariable* CVar)
		{
			if (CVar)
			{
				UE_LOG(LogTemp, Warning, TEXT("콘솔 변수 변경됨! 현재 값: %d"), CVar->GetInt());
				UE_LOG(LogTemp, Warning, TEXT("콘솔 변수 변경됨! GMyTestCoin 값: %d"), GMyTestCoin);

				if (GWorld)
				{
					if (GWorld->GetFirstPlayerController())
					{
						GWorld->GetFirstPlayerController()->GetPlayerState<ACXPlayerState>()->Coins += GMyTestCoin;
					}
				}
				GMyTestCoin = 0;
			}
		}),
	ECVF_Cheat
);

void ACXPlayerState::CTSAddCoins_Implementation(const int32& Amount)
{
	AddCoins(Amount);
}

void ACXPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ThisClass, Coins, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ThisClass, BonusMaxSpeed, COND_OwnerOnly);
}

void ACXPlayerState::AddCoins(int32 Amount)
{
	if (HasAuthority() == false || Amount <= 0)
	{
		return;
	}
	FString PlayerName = GetPlayerName();
	UE_LOG(LogTemp, Warning, TEXT("%s - AddCoins: %d(+%d)"), *PlayerName, Coins, Amount);
	Coins += Amount;
}

bool ACXPlayerState::TryRemoveCoins(int32 Amount)
{
	if (HasAuthority() == false || Amount <= 0 || Coins < 5)
	{
		return false;
	}

	Coins -= Amount;
	return true;
}

void ACXPlayerState::AddBonusMaxSpeed(float Amount)
{
	if (HasAuthority() == false || Amount <= 0.f)
	{
		return;
	}

	BonusMaxSpeed += Amount;
}

FString ACXPlayerState::GetPlayerStateInfo() const
{
	return FString::Printf(TEXT("Coins: %d, BonusMaxSpeeds: %.2f"), Coins, BonusMaxSpeed);
}
