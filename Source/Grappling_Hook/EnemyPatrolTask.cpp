// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPatrolTask.h"

EBTNodeResult::Type UEnemyPatrolTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* Component = &OwnerComp;
	if(!Component)
		return EBTNodeResult::Failed;
	
	AEnemyAI_Controller* AIController = Cast<AEnemyAI_Controller>(Component->GetOwner());
	
	if(!AIController)
		return EBTNodeResult::Failed;

	AIController->RandomEnemyPatrol();
	
	return EBTNodeResult::Succeeded;
}
