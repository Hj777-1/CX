// Copyright Epic Games, Inc. All Rights Reserved.


#include "CXUI.h"
#include "UI/ChatUI.h"
#include "GameState/CXGameState.h"
#include "PlayerState/CXPlayerState.h"
#include "TimerManager.h"

void UCXUI::OnNewChatMessage(const FString& InSender, const FString& InMessage)
{
	UI_Chat->OnNewChatMessage(InSender, InMessage);
}

void UCXUI::UpdateSpeed(float NewSpeed)
{
	// format the speed to KPH or MPH
	float FormattedSpeed = FMath::Abs(NewSpeed) * (bIsMPH ? 0.022f : 0.036f);

	// call the Blueprint handler
	OnSpeedUpdate(FormattedSpeed);
}

void UCXUI::UpdateGear(int32 NewGear)
{
	// call the Blueprint handler
	OnGearUpdate(NewGear);
}

void UCXUI::NativeConstruct()
{
	Super::NativeConstruct();
	TryBindGameState();
}

void UCXUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	ACXGameState* GS = CachedGameState.Get();
	if (GS == nullptr)
	{
		return;
	}

	if (GS->GetRemainingTime() != LastRemainingTime)
	{
		LastRemainingTime = GS->GetRemainingTime();
		HandleRemainingTimeChanged(LastRemainingTime);
	}

	const int32 CurrentState = static_cast<int32>(GS->GetCurrentGameState());
	if (CurrentState != LastGameState)
	{
		LastGameState = CurrentState;
		HandleGameStateChanged(GS->GetCurrentGameState());
	}
}

void UCXUI::NativeDestruct()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(BindRetryHandle);
	}

	if (ACXGameState* GS = CachedGameState.Get())
	{
		GS->OnRemainingTimeChanged.RemoveAll(this);
		GS->OnGameStateChanged.RemoveAll(this);
	}
	Super::NativeDestruct();
}

void UCXUI::TryBindGameState()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	ACXGameState* GS = World->GetGameState<ACXGameState>();
	if (GS == nullptr)
	{
		// 아직 GameState가 복제되지 않았으면 잠시 뒤 재시도
		World->GetTimerManager().SetTimer(BindRetryHandle, this, &UCXUI::TryBindGameState, 0.2f, false);
		return;
	}

	CachedGameState = GS;
	UE_LOG(LogTemp, Warning, TEXT("[UI] bound to GameState"));
	GS->OnRemainingTimeChanged.AddUObject(this, &UCXUI::HandleRemainingTimeChanged);
	GS->OnGameStateChanged.AddUObject(this, &UCXUI::HandleGameStateChanged);

	// 바인딩 시점의 현재 값을 바로 반영 (중간 접속 대비)
	HandleRemainingTimeChanged(GS->GetRemainingTime());
	HandleGameStateChanged(GS->GetCurrentGameState());
}

void UCXUI::HandleRemainingTimeChanged(int32 NewRemainingTime)
{
	UE_LOG(LogTemp, Warning, TEXT("[UI] time=%d"), NewRemainingTime);
	OnRemainingTimeUpdate(NewRemainingTime);
}

void UCXUI::HandleGameStateChanged(EGameState NewGameState)
{
	OnGameStateUpdate(NewGameState);

	if (NewGameState != EGameState::Finished)
	{
		return;
	}

	ACXGameState* GS = CachedGameState.Get();
	ACXPlayerState* MyPS = GetOwningPlayerState<ACXPlayerState>();
	if (GS == nullptr || MyPS == nullptr)
	{
		return;
	}

	const bool bWon = GS->Winners.Contains(MyPS);
	const bool bShared = bWon && GS->Winners.Num() > 1;
	OnGameResult(bWon, bShared);
}