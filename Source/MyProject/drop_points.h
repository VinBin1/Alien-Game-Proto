// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "drop_points.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API Udrop_points : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	Udrop_points();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:

	TArray <AActor*> GetDropPoints() const;

private:
	UPROPERTY(EditInstanceOnly, Category = "Patrol Route")
		TArray <AActor*> DropPoints;
		
};
