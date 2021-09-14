// Fill out your copyright notice in the Description page of Project Settings.

#include "drop_points.h"

// Sets default values for this component's properties
Udrop_points::Udrop_points()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void Udrop_points::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void Udrop_points::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<AActor*> Udrop_points::GetDropPoints() const
{
	return DropPoints;
}

