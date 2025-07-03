// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "Blueprint/StateTreeEvaluatorBlueprintBase.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "JxxStateTreeSomethingBlueprintBase.generated.h"



USTRUCT(BlueprintType)
struct JXXAI_API FBPStateTreeStateInfo
{
	GENERATED_BODY()

	FBPStateTreeStateInfo() = default;

	FBPStateTreeStateInfo(const FCompactStateTreeState& InState)
	{
		Name = InState.Name;
		LinkedState = InState.LinkedState;
		LinkedAsset = InState.LinkedAsset;
		Parent = InState.Parent;
		Type = InState.Type;
		SelectionBehavior = InState.SelectionBehavior;
		bHasTransitionTasks = InState.bHasTransitionTasks;
		bEnabled = InState.bEnabled;
	}

	/** Name of the State */
	UPROPERTY(BlueprintReadOnly)
	FName Name = NAME_None;

	/** Linked state handle if the state type is linked state. */
	UPROPERTY(BlueprintReadOnly)
	FStateTreeStateHandle LinkedState = FStateTreeStateHandle::Invalid;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UStateTree> LinkedAsset = nullptr;

	/** Parent state handle, invalid if root state. */
	UPROPERTY(BlueprintReadOnly)
	FStateTreeStateHandle Parent = FStateTreeStateHandle::Invalid;


	/** Type of the state */
	UPROPERTY(BlueprintReadOnly)
	EStateTreeStateType Type = EStateTreeStateType::State;

	/** What to do when the state is considered for selection. */
	UPROPERTY(BlueprintReadOnly)
	EStateTreeStateSelectionBehavior SelectionBehavior = EStateTreeStateSelectionBehavior::TrySelectChildrenInOrder;

	/** True if the state contains tasks that should be called during transition handling. */
	UPROPERTY(BlueprintReadOnly)
	bool bHasTransitionTasks = false;

	/** True if the state is Enabled (i.e. not explicitly marked as disabled). */
	UPROPERTY(BlueprintReadOnly)
	bool bEnabled = false;
};




UCLASS(Abstract)
class JXXAI_API UJxxStateTreeTaskBlueprintBase : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()


protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)override;

	UFUNCTION(BlueprintCallable, meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool GetStateInfo_Exec(FStateTreeStateHandle InState, FBPStateTreeStateInfo& OutStateInfo)const;

	UFUNCTION(BlueprintCallable)
	FBPStateTreeStateInfo GetStateInfo(FStateTreeStateHandle InState)const;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AActor> ContextActor;

	UPROPERTY(BlueprintReadOnly)
	TArray<FStateTreeEvent> EventToProcess;

	UPROPERTY(BlueprintReadOnly)
	const UStateTree* StateTreeAsset;
	
};


UCLASS(Abstract)
class JXXAI_API UJxxStateTreeEvaluatorBlueprintBase : public UStateTreeEvaluatorBlueprintBase
{
	GENERATED_BODY()


protected:
	virtual void TreeStart(FStateTreeExecutionContext& Context) override;
	virtual void TreeStop(FStateTreeExecutionContext& Context) override;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AActor> ContextActor;

};


UCLASS(Abstract)
class JXXAI_API UJxxStateTreeConditionBlueprintBase : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()


protected:
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

	UPROPERTY(BlueprintReadOnly)
	mutable TWeakObjectPtr<AActor> ContextActor;

};
