// Fill out your copyright notice in the Description page of Project Settings.

#include "dialogue.h"
#include "TextBlock.h"
#include "engine.h"

// Sets default values for this component's properties
Udialogue::Udialogue()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void Udialogue::BeginPlay()
{
	Super::BeginPlay();
	charprog = 0;
	// ...
	
}


// Called every frame
void Udialogue::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



