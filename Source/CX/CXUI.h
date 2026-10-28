// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/TimerHandle.h"
#include "GameState/CXGameState.h"
#include "CXUI.generated.h"

class UChatUI;

/**
 *  Simple Vehicle HUD class
 *  Displays the current speed and gear.
 *  Widget setup is handled in a Blueprint subclass.
 */
UCLASS(abstract)
class CX_API UCXUI : public UUserWidget
{
	GENERATED_BODY()

#pragma region ChatUI

public:
	void OnNewChatMessage(const FString& InSender, const FString& InMessage);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UChatUI> UI_Chat;

#pragma endregion

protected:

	/** Controls the display of speed in Km/h or MPH */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Vehicle)
	bool bIsMPH = false;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	int32 LastRemainingTime = -1;
	int32 LastGameState = -1;
	
public:

	/** Called to update the speed display */
	void UpdateSpeed(float NewSpeed);

	/** Called to update the gear display */
	void UpdateGear(int32 NewGear);

	UFUNCTION(BlueprintImplementableEvent, Category = GameFlow)
	void OnRemainingTimeUpdate(int32 NewRemainingTime);

	UFUNCTION(BlueprintImplementableEvent, Category = GameFlow)
	void OnGameStateUpdate(EGameState NewGameState);

	UFUNCTION(BlueprintImplementableEvent, Category = GameFlow)
	void OnGameResult(bool bWon, bool bShared);

protected:

	/** Implemented in Blueprint to display the new speed */
	UFUNCTION(BlueprintImplementableEvent, Category = Vehicle)
	void OnSpeedUpdate(float NewSpeed);

	/** Implemented in Blueprint to display the new gear */
	UFUNCTION(BlueprintImplementableEvent, Category = Vehicle)
	void OnGearUpdate(int32 NewGear);

private:
	void TryBindGameState();
	void HandleRemainingTimeChanged(int32 NewRemainingTime);
	void HandleGameStateChanged(EGameState NewGameState);

	TWeakObjectPtr<ACXGameState> CachedGameState;
	FTimerHandle BindRetryHandle;

};
