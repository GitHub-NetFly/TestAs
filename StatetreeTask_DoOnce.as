UCLASS(meta = (DisplayName = "DoOnce", Category = "ConditionTask"))
class UStateTreeTask_DoOnce : UStateTreeTaskAngelScript
{
	UPROPERTY()
	bool bHasExecuted = false;

	UFUNCTION(BlueprintOverride)
	void LatentEnterState(FStateTreeTransitionResult Transition)
	{
		Super::LatentEnterState(Transition);

		if (bHasExecuted)
		{
			FinishTask(false);
			return;
		}

		bHasExecuted = true;
		FinishTask(true);
	}
}