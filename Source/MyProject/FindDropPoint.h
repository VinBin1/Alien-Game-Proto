// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FindDropPoint.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UFindDropPoint : public UBTTaskNode
{
	GENERATED_BODY()
		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
		struct FBlackboardKeySelector indexval1;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
		struct FBlackboardKeySelector droppointkey;

	UPROPERTY(EditInstanceOnly, Category = "Patrol Route")
		TArray <float> DropPointsDistance;
};
