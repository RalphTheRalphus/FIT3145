// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EnemyAI_Controller.h"
#include "EnemyPatrolTask.generated.h"

/**
 * 
 */
UCLASS()
class GRAPPLING_HOOK_API UEnemyPatrolTask : public UBTTaskNode
{
	GENERATED_BODY()
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
