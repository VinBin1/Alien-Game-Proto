// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MyProjectCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Blueprint/UserWidget.h"
#include "dialogue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "engine.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// AMyProjectCharacter
void AMyProjectCharacter::BeginPlay()
{
	
	
	Super::BeginPlay();
	
	printshit("welcome..");
	centerText = "";
	
	firethistime = true;
	ismorphed = false;
	HasBeenSeen = false;
	no_rocklingsrescued = 0;
	has_Rockling = false;
	MoveisDisabled = false;
	XboxConOn = true;
	

	//create dynamic material, set parameter value and set mesh mat.
	DynMaterial=UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
	
	DynMaterial->SetScalarParameterValue("CHANGE", 0.0f);
	GetMesh()->SetMaterial(0, DynMaterial);
	flipmorph();//start as rock?
	//populate array of guard/npc actors
	//TSubclassOf<AActor> ClassToFind;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, Npcs);
	
}

void AMyProjectCharacter::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime); 

	CheckShadow();

	
	if (camisrot)//and ? check.
	{
		printshit("Caught");
		RotateToSpotter();
	}
	//tick
}

AMyProjectCharacter::AMyProjectCharacter()
{
	//PrimaryActorTick.bCanEverTick = true;

	//controler vars
	lookingleft = false;
	//
	//dialog vars
	charnamestr = "";
	theactor = NULL;
	talkingnow = false;
	dialogue = NULL;
	charprogvar = 0;
	cantalknow = false;

	hud_is_on = false;
	//spottercam
	spotter = NULL;
	camisrot = false;
	PlayerCapsule= GetCapsuleComponent();

	isBeingCarried = false;
	isRockbutOuttaWay = false;
	// linecaster object set up
	castpointcube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CastpointCube"));
	FVector offsetvec(0, 0, 1.f);
	castpointcube->SetupAttachment(RootComponent);
	

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 350.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMyProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("showpause", IE_Released, this, &AMyProjectCharacter::showDB);
	PlayerInputComponent->BindAction("Lookleftbtn", IE_Pressed, this, &AMyProjectCharacter::justlookleft);//leftbumper
	PlayerInputComponent->BindAction("Lookleftbtn", IE_Released, this, &AMyProjectCharacter::justlookleft);
	PlayerInputComponent->BindAction("rightbumper", IE_Pressed, this, &AMyProjectCharacter::flipmorph);//right bumper-morph?
	PlayerInputComponent->BindAction("xbutton", IE_Pressed, this, &AMyProjectCharacter::sprint);
	PlayerInputComponent->BindAction("xbutton", IE_Released, this, &AMyProjectCharacter::sprint);
	PlayerInputComponent->BindAction("startButton", IE_Released, this, &AMyProjectCharacter::startPressed);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyProjectCharacter::MoveRight);
	
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMyProjectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyProjectCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMyProjectCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMyProjectCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMyProjectCharacter::OnResetVR);
}

bool AMyProjectCharacter::gethud_is_on()
{
	return 	hud_is_on;
	printshit("E pressed");
}

 void AMyProjectCharacter::showDB()
{
	 //db
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("bool is: %s"), hud_is_on ? TEXT("true") : TEXT("false")));
	if (talkingnow == false)
	{
		hud_is_on = true;
		printshit("hudison flipped");
	}
	return;
}
 void AMyProjectCharacter::sprint()
 {
	 printshit("sprint pressed");
 }
 


#pragma region old dialogue methods


 void AMyProjectCharacter::doraycast()//Dialogue move?
 {

	 if (hud_is_on == false)
	 {

		 if (talkingnow == false)
		 {
			 FHitResult* HitResult = new FHitResult();
			 FVector StartTrace = castpointcube->GetComponentLocation();
			 FVector ForwardVector = castpointcube->GetForwardVector();

			 FVector EndTrace = ((ForwardVector*500.f) + StartTrace);
			 FCollisionQueryParams* TraceParams = new FCollisionQueryParams();



			 //raycastlinetrace
			 if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
			 {

				 charnamestr = *HitResult->Actor->GetName();


				 //if valid char WTF
				 if ((charnamestr == FString(TEXT("SpaceLady"))) || (charnamestr == FString(TEXT("Dude"))) || (charnamestr == FString(TEXT("JohntheGuard"))))
				 {
					 printshit("PRESS E to talk");
					 if (charnamestr != "")
					 {
						 GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CHAR HIT hit : %s"), *charnamestr));
					 }
					 theactor = Cast<AActor>(HitResult->GetActor());
					 dialogue = theactor->FindComponentByClass<Udialogue>();
					 cantalknow = true;
					 charprogvar = dialogue->charprog;

					 //fill widget string with array
					 reloadtext();

					 talkingnow = true;
				 }
				 else {
					 talkingnow = false;
					 cantalknow = false;
				 }
				 talkingnow = false;
			 }

		 }
	 }


 }


 bool AMyProjectCharacter::flipcantalk()
 {
	 cantalknow = !cantalknow;
	 return cantalknow;
 }

 int AMyProjectCharacter::incrementcon()
 {
	 charprogvar = charprogvar + 5;

	 GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("increment charprogvar")));
	 GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("char var is : %d"), charprogvar));
	 reloadtext();

	 return charprogvar;
 }

 void AMyProjectCharacter::reloadtext()
 {
	 GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("text reload")));
	 //loads text in array
	 charsaystr = dialogue->charlines[charprogvar];
	 firstline = dialogue->charlines[charprogvar + 1];
	 secondline = dialogue->charlines[charprogvar + 2];
	 thirdline = dialogue->charlines[charprogvar + 3];
	 dialogue->charprog = charprogvar;

 }


 FString AMyProjectCharacter::getfirstline()
 {
	 return firstline;
 }

 FString AMyProjectCharacter::getsecondline()
 {
	 return secondline;
 }

 FString AMyProjectCharacter::getthirdline()
 {
	 return thirdline;
 }

 FString AMyProjectCharacter::getcharname()
 {
	 return charnamestr;
 }

 FString AMyProjectCharacter::getCenterText()
 {
	 return centerText;

 }


 int AMyProjectCharacter::closeDB()
 {
	 theactor = NULL;
	 cantalknow = true;
	 talkingnow = false;
	 printshit("close DB, hudisonfalse?");
	 hud_is_on = false;
	 return 0;
 }

 FString AMyProjectCharacter::getcharsaystr()
 {
	 return charsaystr;
 }

 int AMyProjectCharacter::getcharnum()
 {
	 return charprogvar;
 }

 void AMyProjectCharacter::lineCastTo()
 {
	 if (LadyChar)
	 {
		 FVector playerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
		 FVector ActorLocation = LadyChar->GetActorLocation();

		 //FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
		 FHitResult HitResult;

		 FCollisionQueryParams TraceParams(
			 FName(TEXT("Tracelady")),	// tag name (for debugging)
			 true,				// trace against simple collision primitives only
			 this				// ignore this actor during the trace
		 );

		 //raycastlinetrace
		 DrawDebugLine(GetWorld(), playerLocation, ActorLocation, FColor::Yellow, false, -1, 0, 1.0f);

		 if (GetWorld()->LineTraceSingleByChannel(HitResult, playerLocation, ActorLocation, ECC_Visibility, TraceParams))
		 {

			 charnamestr = HitResult.GetActor()->GetName();
			 if (charnamestr != "")
			 {
				 printshit(charnamestr);
			 }
			 //if linetrace and googles off
			 /*if (charnamestr == FString(TEXT("ladypawn")))
			 {
				 printshit("line of sight to actor");
			 }

			 else {
				 printshit("NO LINE O SIGHT TO");
			 }*/
		 }
		 else {
			 printshit("line of sight to actor");
		 }


	 }
 }

#pragma endregion



 //utility
FString AMyProjectCharacter::printshit(FString sometext)
{
	const TCHAR *mystring = *sometext;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, mystring);
	return FString();
}


#pragma region Standard Input code
void AMyProjectCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMyProjectCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{


}

void AMyProjectCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AMyProjectCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyProjectCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMyProjectCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && (ismorphed == false) && (MoveisDisabled == false))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMyProjectCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && (ismorphed == false) && (MoveisDisabled == false))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

#pragma endregion



#pragma region UI
//UI
FString AMyProjectCharacter::blankIntroText()
{
	centerText = "";
	return "";
}

FString AMyProjectCharacter::setAlertText(FString sometext)
{
	AlertText = sometext;

	return AlertText;
}

FString AMyProjectCharacter::GetAlertText()
{
	return AlertText;
}

bool AMyProjectCharacter::ShowHideXboxcon()//rename unclear is UI
{
	return XboxConOn;
}
#pragma endregion


bool AMyProjectCharacter::getbIsInShadow(bool yepno)
{
	return bIsInShadow;
}

bool AMyProjectCharacter::CheckShadow()
{
	//check and set in shadow 
	//requires line trace
	
	FCollisionQueryParams QueryParams=new FCollisionQueryParams();
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	FHitResult* Hit= new FHitResult();

	FVector playerpos = GetActorLocation();
	FVector lightpos = SunLight->GetActorForwardVector();
	FVector offset = FVector(-1000.0f, -1000.0f,-1000.0f);
	lightpos = lightpos * offset;

	FVector Endpos =  playerpos+lightpos ;
	

		if (!GetWorld()->LineTraceSingleByChannel(*Hit,  playerpos, Endpos, ECC_Visibility, QueryParams))
		{
			
			bIsInShadow = false;
	    }
		else {
			
			bIsInShadow = true;
		}
	//with loc light ref and player 

	return false;
}


EPlayerStatus AMyProjectCharacter::checkPlayerStatus(EPlayerStatus somestatus)//return status called by ai 
{

	if (bIsInShadow == true)
	{
		return EPlayerStatus::inshadow;
	}
	else {
			if (ismorphed == false)
			{
			return EPlayerStatus::NotRockandNotinShadow;
			}
			else
			{
				if (isBeingCarried == true)
				{
					return EPlayerStatus::isBeingCarried;
				}

				if (ismorphed == true && isRockbutOuttaWay == false)
				{
					return EPlayerStatus::isRockandInway;
				}
				else {
					if (ismorphed == true && isRockbutOuttaWay == true)
					{
					return EPlayerStatus::isRockbutNotinWay;
					}
					else {
						return EPlayerStatus::isRockbutNotinWay;
					}

				}
			}

	}
	
}

bool AMyProjectCharacter::getlookleft()
{
	
	return lookingleft;
	
	
}

bool AMyProjectCharacter::getmorph()
{

		return ismorphed;
}

bool AMyProjectCharacter::getIsCarried()
{
	return isBeingCarried;
}

bool AMyProjectCharacter::getisRockbutOuttaWay()
{
	return isRockbutOuttaWay;
}

bool AMyProjectCharacter::setisRockbutOuttaWay( bool outta)
{
	isRockbutOuttaWay = outta;
	return true;
}

bool AMyProjectCharacter::SetIsCarried( bool iscarryied)
{
	isBeingCarried = iscarryied;

	return true;
}

bool AMyProjectCharacter::GetHasBeenSeen()
{

	return HasBeenSeen;
}

bool AMyProjectCharacter::setHasBeenSeen()
{
	HasBeenSeen = true;
	printshit("has been seen set to " + HasBeenSeen);
	CameraBoom->bUsePawnControlRotation = false;
	
	MoveisDisabled = true;
	camisrot = true;
	return true;
}

void AMyProjectCharacter::flipmorph()
{
	
	ismorphed = !ismorphed;
	morphint++;

	if (morphint == 2)
	{
		printshit("First Morph!");//TODO call introtext
		centerText = "AH! Your Awake!  \n You must rescue the other Rocklings from the Humans  \n And you must do so WITHOUT them knowing of our presence.";
		
		XboxConOn = false;
	}

	if (ismorphed == true)
	{
		DynMaterial->SetScalarParameterValue("CHANGE", 1.0f);
	}
	else {

		DynMaterial->SetScalarParameterValue("CHANGE", 0.0f);
	}

	GetMesh()->SetMaterial(0, DynMaterial);

	//disable jump hack
	if (ismorphed == true)
	{
		//Jump();
		GetCharacterMovement()->JumpZVelocity = 0.f;
		PlayerCapsule->SetCapsuleSize(25.5f, 50.5f, true);
	}
	else {
		GetCharacterMovement()->JumpZVelocity = 600.f;
		PlayerCapsule->SetCapsuleSize(36.5f, 70.5f, true);
	}
}

void AMyProjectCharacter::justlookleft()
{

	lookingleft =!lookingleft;
	
	if (lookingleft)
	{
		
		//lookat
		FVector MyLocation = GetActorLocation();
		
	
		if (spotter!=NULL)
		{
			FVector lookLocation = spotter->GetActorLocation();
			
			FVector direction =  lookLocation - MyLocation;
			direction.Normalize();

		}
	}
	else {
		CameraBoom->TargetArmLength = 350.0f;
		CameraBoom->TargetOffset.X = 0.0f;
	}
}

void AMyProjectCharacter::startPressed()
{
	printshit("START Pressed");
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}



void AMyProjectCharacter::RotateToSpotter()
{
	FVector Playerloc = GetActorLocation();
	FVector spotterloc = spotter->GetActorLocation();
	FVector Dir = spotterloc - Playerloc;
	
	Dir.Normalize();
	
	SetActorRotation(FMath::Lerp(GetActorRotation(), Dir.Rotation(), 0.05f));

	FRotator Rot = FRotationMatrix::MakeFromX(Dir).Rotator();//??
	CameraBoom->SetWorldRotation(FMath::Lerp(CameraBoom->GetComponentRotation(), Dir.Rotation(), 0.0001f));
	

}

void AMyProjectCharacter::EndTheGame()
{
	//TODO
	//showtest press start to restart/
	//delay
	//show gameovertext
	FTimerHandle dummyHandle;
	GetWorldTimerManager().SetTimer(dummyHandle, this, &AMyProjectCharacter::RestartGame, 0.5f, false);
	//delay
	//restartgame
}
void AMyProjectCharacter::RestartGame()
{

}