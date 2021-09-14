// Fill out your copyright notice in the Description page of Project Settings.
#include "FindDropPoint.h"
#include "drop_points.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "AIController.h"


EBTNodeResult::Type UFindDropPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto points = ControlledPawn->FindComponentByClass<Udrop_points>();
	
	FVector playerloc= GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	ACharacter* myCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (!ensure(points))
	{
		return EBTNodeResult::Failed;
	}

	auto theDropPoints = points->GetDropPoints();

	//iterate through array of drop points, find the nearest, assign to blackboard
	//acess tarray in points object?
	for (int i = 0; i < theDropPoints.Num(); i++)
	{
		//theDropPoints
		FVector ThisElementVector = theDropPoints[i]->GetActorLocation();
			float thisElementDistance = (ThisElementVector - playerloc).Size();
			DropPointsDistance[i] = thisElementDistance;

	//float thiselementDistanceVector=playerpoint-elementVector
	}
	int index = 0;

	for (int i = 1; i < theDropPoints.Num(); i++)
	{
		if (DropPointsDistance[i] < DropPointsDistance[index])
			index = i;
	}
	AActor* closestpoint = theDropPoints[index];
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("nearest drop point %s"), *closestpoint->GetName()));
	//DropPointsDistance.Sort();

	return EBTNodeResult::Failed;
}