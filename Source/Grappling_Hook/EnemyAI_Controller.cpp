// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI_Controller.h"
#include "Enemy.h"
#include "GameFramework/Character.h"

AEnemyAI_Controller::AEnemyAI_Controller()
{
	PrimaryActorTick.bCanEverTick = true;
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
	
	//Set sight
	SightConfig->SightRadius = SightRadius;
	SightConfig->SetMaxAge(SightAge);
	SightConfig->LoseSightRadius = SightLoseRadius;
	SightConfig->PeripheralVisionAngleDegrees = FieldOfView;

	//set friendlies and enemies
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	//set dominant sense
	GetPerceptionComponent()->SetDominantSense(SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	GetPerceptionComponent()->ConfigureSense(*DamageConfig);
	
	//On perception updated
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAI_Controller::OnSensesUpdated);

}

void AEnemyAI_Controller::BeginPlay()
{
	Super::BeginPlay();
	if(!BlackboardData)
		return;
	if(!BehaviorTree)
		return;

	UseBlackboard(BlackboardData, BlackboardComponent);
	RunBehaviorTree(BehaviorTree);

	NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	BlackboardComponent->SetValueAsBool("Patrol", true);
	RandomEnemyPatrol();

	AEnemy* Enemy = Cast<AEnemy>(GetPawn());
	if(Enemy)
	{
		WeaponRot = Enemy->GetMesh()->GetSocketRotation("EnemyWeapon");
		AttackDist = Enemy->AttackDistance;
		if(Enemy->EnemyWeaponClass)
		{
			EnemyWeapon = GetWorld()->SpawnActor<ADamageOnOverlap>(Enemy->EnemyWeaponClass);
			//Enemy->EnemyWeapon->AttachToActor(Enemy, FAttachmentTransformRules::KeepRelativeTransform, "EnemyWeapon");
		}
	}
}

void AEnemyAI_Controller::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(Player)
	{
		BlackboardComponent->SetValueAsVector("PlayerLoc", Player->GetActorLocation());
		float dist = FVector::Dist(GetPawn()->GetActorLocation(), Player->GetActorLocation());
		if(dist < AttackDist)
		{
			BlackboardComponent->SetValueAsBool("Chase", false);
			BlackboardComponent->SetValueAsBool("Attack", true);
			BlackboardComponent->SetValueAsBool("Patrol", false);
		}
		else
		{
			BlackboardComponent->SetValueAsBool("Chase", true);
			BlackboardComponent->SetValueAsBool("Attack", false);
		}
		
		BlackboardComponent->SetValueAsVector("LastKnownLoc", Player->GetActorLocation());
	}
}

void AEnemyAI_Controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AEnemyAI_Controller::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
}

FRotator AEnemyAI_Controller::GetControlRotation() const
{
	Super::GetControlRotation();
	if(GetPawn())
	{
		return FRotator(0, GetPawn()->GetActorRotation().Yaw, 0); 
	}
	return FRotator(0,0,0);
}

void AEnemyAI_Controller::RandomEnemyPatrol()
{
	if(NavSystem)
	{
		FNavLocation RandomLoc;
		NavSystem->GetRandomPointInNavigableRadius(GetPawn()->GetActorLocation(), 3000, RandomLoc);
		BlackboardComponent->SetValueAsVector("PatrolPoint", RandomLoc.Location);
	}
}

void AEnemyAI_Controller::EnemyAttack()
{
	if(NiagaraSystem && Player)
	{
		FRotator FacePlayer = (Player->GetActorLocation() - GetPawn()->GetActorLocation()).Rotation();

		FRotator FaceRotSpeed = FMath::Lerp(GetPawn()->GetActorRotation(), FRotator(0, FacePlayer.Yaw, 0), 0.5);
		GetPawn()->SetActorRotation(FaceRotSpeed);

		AEnemy* EnemyCharacter = Cast<AEnemy>(GetPawn());
		if(EnemyCharacter)
		{
			FVector weaponLoc =  EnemyCharacter->GetMesh()->GetSocketLocation("EnemyWeapon");
			FRotator AimRot = (Player->GetActorLocation() - weaponLoc).Rotation();
			FRotator AimRotSpeed = FMath::RInterpTo(WeaponRot, AimRot, FApp::GetDeltaTime(), EnemyCharacter->AttackSpeed);
			WeaponRot = AimRotSpeed;
			// damage on overlap cast
			
			if(EnemyWeapon)
			{
				UNiagaraComponent* SpawnEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), EnemyWeapon->NiagaraSystem, weaponLoc, AimRotSpeed,
				FVector(1,1,1));
				EnemyWeapon->SetActorRotation(AimRotSpeed);
				EnemyWeapon->SetActorLocation(weaponLoc);
			}

		}
	}
}

void AEnemyAI_Controller::OnSensesUpdated(AActor* SensedActor, FAIStimulus Stimulus)
{
	APawn* TargetPlayer = Cast<APawn>(SensedActor);
	UE_LOG(LogTemp, Warning, TEXT("Stimulus Type Name: %s"), *Stimulus.Type.Name.ToString());
	if(TargetPlayer && TargetPlayer->IsPlayerControlled())
	{
		if(Stimulus.WasSuccessfullySensed())
		{
			Player = TargetPlayer;
			BlackboardComponent->SetValueAsBool("Patrol", false);
		}
		else if(Player && !LineOfSightTo(Player))
		{
			Player = nullptr;
			BlackboardComponent->ClearValue("PlayerLoc");
			BlackboardComponent->SetValueAsBool("Attack", false);
			BlackboardComponent->SetValueAsBool("Chase", false);
			BlackboardComponent->SetValueAsBool("Patrol", true);
			BlackboardComponent->SetValueAsBool("Investigate", true);
		}
	}
}
