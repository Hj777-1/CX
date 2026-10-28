// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/MultiLineEditableTextBox.h"
#include "ChatUI.generated.h"

class UMultiLineEditableTextBox;

/**
 * 
 */
UCLASS()
class CX_API UChatUI : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void OnNewChatMessage(const FString& InSender, const FString& InMessage); //새로운 메세지가 들어왔을 때 호출할 함수
	
protected:	
	virtual void NativeConstruct() override; // UI가 만들어졌을 때
	virtual void NativeDestruct() override; // 제거 됐을 때 


	UFUNCTION()
	void OnChatInputTextCommitted(const FText& text, ETextCommit::Type CommitMethod); // 채팅창에서 엔터키를 눌렀을때 


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox_ChatInput;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ChatScrollBox;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMultiLineEditableTextBox> MultilineEditableTextBox_ChatLog;


};
