// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DamageOnOverlap.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NavigationSystem.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AIPerceptionComponent.h"
#include "EnemyAI_Controller.generated.h"

/**
 * 
 */
UCLASS()
class GRAPPLING_HOOK_API AEnemyAI_Controller : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAI_Controller();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPossess(APawn* InPawn) override;
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result) override;
	virtual FRotator GetControlRotation() const override;
	void RandomEnemyPatrol();
	void EnemyAttack();

	UFUNCTION()
	void OnSensesUpdated(AActor* SensedActor, FAIStimulus Stimulus);
	UPROPERTY(EditDefaultsOnly, Category = "Sight")
	float SightRadius = 3000;
	UPROPERTY(EditDefaultsOnly, Category = "Sight")
	float FieldOfView = 80;
	UPROPERTY(EditDefaultsOnly, Category = "Sight")
	float SightLoseRadius = 1100;
	UPROPERTY(EditDefaultsOnly, Category = "Sight")
	float SightAge = 5;
	UPROPERTY(EditDefaultsOnly, Category = "Sight")
	UAISenseConfig_Sight* SightConfig;


	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	UAISenseConfig_Damage* DamageConfig;
	
	UNavigationSystemV1* NavSystem;
	APawn* Player;
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	UBlackboardData* BlackboardData;
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	UBlackboardComponent* BlackboardComponent;
	UPROPERTY(EditDefaultsOnly, Category = "BehaviurTree")
	UBehaviorTree* BehaviorTree;

	//Niagara for attacks
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UNiagaraSystem* NiagaraSystem;

	//Aim Start and attack distance
	FRotator WeaponRot;
	float AttackDist;
	ADamageOnOverlap* EnemyWeapon;
};
