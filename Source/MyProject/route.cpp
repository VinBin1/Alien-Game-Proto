// Fill out your copyright notice in the Description page of Project Settings.

#include "route.h"

// Sets default values for this component's properties
Uroute::Uroute()
{
	
}


// Called when the game starts
void Uroute::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}




TArray<AActor*> Uroute::GetPatrolPoints() const
{
	return PatrolPoints;
}

