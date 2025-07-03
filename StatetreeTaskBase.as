class UStateTreeTaskAngelScript : UJxxStateTreeTaskBlueprintBase
{
#if EDITOR
	FName SourceRootStateName;
	FName SourceStateName;
	FName TargetStateName;
	FName CurrentStateName;
#endif

	UFUNCTION(BlueprintOverride)
	void LatentEnterState(FStateTreeTransitionResult Transition)
	{
#if EDITOR
		SourceRootStateName = GetStateInfo(Transition.SourceRootState).Name;
		SourceStateName = GetStateInfo(Transition.SourceState).Name;
		TargetStateName = GetStateInfo(Transition.TargetState).Name;
		CurrentStateName = GetStateInfo(Transition.CurrentState).Name;
#endif
	}
}