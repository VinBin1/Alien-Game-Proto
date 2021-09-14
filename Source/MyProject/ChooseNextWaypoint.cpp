// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "route.h"
#include "AIController.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// TODO protect against no patrol points empty or    TODO no component

	//get the patrol points
	//auto AIController = OwnerComp.GetAIOwner();
	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto PatrolRoute = ControlledPawn->FindComponentByClass<Uroute>();

	if (!ensure(PatrolRoute))
	{
		return EBTNodeResult::Failed;
	}

	auto PatrolPoints = PatrolRoute->GetPatrolPoints();

	if (PatrolPoints.Num() == 0)
	{
		return EBTNodeResult::Failed;
	}

	//set next waypoint
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto index = BlackboardComp->GetValueAsInt(indexval.SelectedKeyName);
	BlackboardComp->SetValueAsObject(waypointkey.SelectedKeyName, PatrolPoints[index]);

	UE_LOG(LogTemp, Warning, TEXT("WayPoint Index: %i"), index);

	//cycle the index
	auto NextIndex = (index + 1) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(indexval.SelectedKeyName,NextIndex);

	return EBTNodeResult::Succeeded;
}