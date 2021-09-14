// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "route.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API Uroute : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	Uroute();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	TArray <AActor*> GetPatrolPoints() const;

private:
	UPROPERTY(EditInstanceOnly, Category = "Patrol Route")
		TArray <AActor*> PatrolPoints;
		
};
