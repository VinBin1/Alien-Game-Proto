// Fill out your copyright notice in the Description page of Project Settings.

#include "cameraswitcher.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
Acameraswitcher::Acameraswitcher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Acameraswitcher::BeginPlay()
{
	Super::BeginPlay();
	}

// Called every frame
void Acameraswitcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const float Timebetweenchanges = 5.0f;
	const float smoothblendtime = 4.0f;

	waittimeforchange -= DeltaTime;

	if (waittimeforchange <= 0.0f)
	{
		waittimeforchange = Timebetweenchanges;
		APlayerController* Ourplayer = UGameplayStatics::GetPlayerController(this, 0);
	
		if (Ourplayer)
		{
			if (Cameratwo && (Ourplayer->GetViewTarget() == Cameraone))
			{
				Ourplayer->SetViewTargetWithBlend(Cameratwo, smoothblendtime);

			}
			else if (Cameraone)
			{
				Ourplayer->SetViewTarget(Cameraone);
			}

		}



	}
}

