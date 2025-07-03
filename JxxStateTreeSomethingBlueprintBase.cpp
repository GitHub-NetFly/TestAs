// Fill out your copyright notice in the Description page of Project Settings.


#include "JxxStateTreeSomethingBlueprintBase.h"

#include "StateTreeExecutionContext.h"

static TArray<FStateTreeEvent> GetEventToProcess(const FStateTreeExecutionContext& Context)
{
	TArray<FStateTreeEvent> Result;

	for (const FStateTreeSharedEvent& Event: Context.GetEventsToProcessView())
	{
		Result.Add(*Event);
	}

	return Result;
}





EStateTreeRunStatus UJxxStateTreeTaskBlueprintBase::EnterState(FStateTreeExecutionContext& Context,const FStateTreeTransitionResult& Transition)
{
	ContextActor = GetOwnerActor(Context);
	EventToProcess = GetEventToProcess(Context);
	StateTreeAsset = Context.GetStateTree();


	return Super::EnterState(Context, Transition);
}

EStateTreeRunStatus UJxxStateTreeTaskBlueprintBase::Tick(FStateTreeExecutionContext& Context,const float DeltaTime)
{
	EventToProcess = GetEventToProcess(Context);

	return Super::Tick(Context, DeltaTime);
}

void UJxxStateTreeTaskBlueprintBase::ExitState(FStateTreeExecutionContext& Context,const FStateTreeTransitionResult& Transition)
{
	EventToProcess = GetEventToProcess(Context);

	Super::ExitState(Context, Transition);

	ContextActor = nullptr;
	EventToProcess.Empty();
	StateTreeAsset = nullptr;
}

bool UJxxStateTreeTaskBlueprintBase::GetStateInfo_Exec(FStateTreeStateHandle InState, FBPStateTreeStateInfo& OutStateInfo) const
{
	OutStateInfo = {};
	if (StateTreeAsset)
	{
		const FCompactStateTreeState* Ptr = StateTreeAsset->GetStateFromHandle(InState);
		if (Ptr)
		{
			OutStateInfo = FBPStateTreeStateInfo(*Ptr);
			return true;
		}
	}
	return false;
}

FBPStateTreeStateInfo UJxxStateTreeTaskBlueprintBase::GetStateInfo(FStateTreeStateHandle InState) const
{
	FBPStateTreeStateInfo StateInfo;
	if (StateTreeAsset)
	{
		const FCompactStateTreeState* Ptr = StateTreeAsset->GetStateFromHandle(InState);
		if (Ptr)
		{
			StateInfo = FBPStateTreeStateInfo(*Ptr);
		}
	}
	return StateInfo;
}

void UJxxStateTreeEvaluatorBlueprintBase::TreeStart(FStateTreeExecutionContext& Context)
{
	ContextActor = GetOwnerActor(Context);


	Super::TreeStart(Context);
}

void UJxxStateTreeEvaluatorBlueprintBase::TreeStop(FStateTreeExecutionContext& Context)
{
	Super::TreeStop(Context);

	ContextActor = nullptr;
}

bool UJxxStateTreeConditionBlueprintBase::TestCondition(FStateTreeExecutionContext& Context) const
{
	if (bHasTestCondition)
	{
		// Cache the owner and event queue for the duration the condition is evaluated.
		SetCachedInstanceDataFromContext(Context);

		ContextActor = GetOwnerActor(Context);

		const bool bResult = ReceiveTestCondition();

		ContextActor = nullptr;

		ClearCachedInstanceData();

		return bResult;
	}
	return false;
}