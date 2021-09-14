// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "thePlayerController.h"
#include "dialogue.h"
#include "Blueprint/UserWidget.h"
#include "MyProjectCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerStatus:uint8
{
	inshadow,
	NotRockandNotinShadow,
	isRockandInway,
	isRockbutNotinWay,
	isBeingCarried

};



UCLASS(config=Game)
class AMyProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	


public:
	void doraycast();

	void lineCastTo();

	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
		AActor* LadyChar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> Npcs;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		EPlayerStatus PlayerStatusEnum;

	UFUNCTION(BlueprintCallable, Category = "UIStuff")
		FString blankIntroText();

	UFUNCTION(BlueprintCallable, Category = "UIStuff")
		FString setAlertText(FString sometext);

	UFUNCTION(BlueprintCallable, Category = "UIStuff")
		FString GetAlertText();

	UFUNCTION(BlueprintCallable, Category = "Shadow")
		bool getbIsInShadow(bool yepno);

	
	//ui stuff
	UFUNCTION(BlueprintPure, Category = "line")
		FString getfirstline();

	UFUNCTION(BlueprintPure, Category = "line")
		FString getcharsaystr();

	UFUNCTION(BlueprintPure, Category = "line")
		FString getsecondline();

	UFUNCTION(BlueprintPure, Category = "line")
		FString getthirdline();

	UFUNCTION(BlueprintPure, Category = "line")
		FString getcharname();

	UFUNCTION(BlueprintPure, Category = "none")
		FString getCenterText();

	UFUNCTION(BlueprintCallable, Category = "line")
		int closeDB();

	UFUNCTION(BlueprintCallable, Category = "checks")
		bool CheckShadow();

	//UFUNCTION(BlueprintCallable, Category = "spotter")
	//	FVector getspotvector();

	UPROPERTY(EditAnywhere)
		AActor* CameraOne;

	UPROPERTY(EditAnywhere)
	AActor* SunLight;

	UPROPERTY(EditAnywhere)
		bool camisrot;

	UPROPERTY(EditAnywhere)
		bool bIsInShadow;

	UPROPERTY(EditAnywhere)
		int morphint;//to trigger intro text

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rock)
		bool has_Rockling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rock)
		int no_rocklingsrescued;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		FString AlertText;

	/**player begin*/
		virtual void BeginPlay() override;
		//tick
		virtual void Tick(float DeltaTime) override;

	AMyProjectCharacter();
	
	UPROPERTY(EditAnywhere, Category = none)
		AthePlayerController* thePlayerController;

	//UPROPERTY(EditAnywhere, Category = none)
	//	APlayerController* Ourplayercon;

	UPROPERTY(EditAnywhere, Category = none)
		UCapsuleComponent* PlayerCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = raycastpoint)
		UStaticMeshComponent* castpointcube;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = raycastpoint)
		Udialogue* dialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = raycastpoint)
		AActor* spotter;

	

	//UPROPERTY(EditAnywhere) 
	//	UMaterialInstance* Material;
	

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	

	UFUNCTION(BlueprintPure, Category = "tog")
		bool gethud_is_on();

	UFUNCTION(BlueprintPure, Category = "tog")
		bool getlookleft();

	UFUNCTION(BlueprintPure, Category = "tog")
		bool getmorph();

	UFUNCTION(BlueprintPure, Category = "tog")
		bool getIsCarried();

	UFUNCTION(BlueprintPure, Category = "tog")
	bool getisRockbutOuttaWay();

	UFUNCTION(BlueprintPure, Category = "tog")
		bool setisRockbutOuttaWay(bool outta);

	UFUNCTION(BlueprintPure, Category = "tog")
		bool SetIsCarried(bool iscarryied);

	UFUNCTION(BlueprintPure, Category = "tog")
		bool GetHasBeenSeen();

	UFUNCTION(BlueprintPure, Category = "tog")
		bool ShowHideXboxcon();

	UFUNCTION(BlueprintPure, Category = "tog")
		bool setHasBeenSeen();

	UFUNCTION()
		void flipmorph();

	UFUNCTION()
		void justlookleft();

	UFUNCTION()
		void startPressed();

	UFUNCTION()
		void RotateToSpotter();

	UFUNCTION()
		void EndTheGame();

	UFUNCTION()
		void RestartGame();

	UFUNCTION(BlueprintPure, Category = "tog")
		int getcharnum();

	UFUNCTION(BlueprintPure, Category = "stealth")
		EPlayerStatus checkPlayerStatus(EPlayerStatus somestatus);

	UFUNCTION()
		void showDB();

	UFUNCTION()
		void sprint();

	UFUNCTION(BlueprintPure, Category = "tog")
		bool flipcantalk();

	UFUNCTION(BlueprintCallable, Category = "tog")
		int incrementcon();

	UFUNCTION()
		void reloadtext();

	UFUNCTION()
		FString printshit(FString sometext);

	
	

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	UPROPERTY(EditAnywhere, Category = "none")
		bool firethistime;

private:
		UPROPERTY(EditAnywhere, Category = "tog")
		bool hud_is_on;

		UPROPERTY(EditAnywhere, Category = "line")
			FString centerText;

		UPROPERTY(EditAnywhere, Category = "Timer")
			FTimerHandle timerhandler;

		UPROPERTY(EditAnywhere, Category = "none")
			bool lookingleft;

		UPROPERTY(EditAnywhere, Category = "none")
			bool MoveisDisabled;

		UPROPERTY(EditAnywhere, Category = "tog")
			bool cantalknow;
		UPROPERTY(EditAnywhere, Category = "tog")
			bool talkingnow;

		UPROPERTY(EditAnywhere, Category = "tog")
			AActor* theactor;//talking to
		
		UPROPERTY(EditAnywhere, Category = "line")
			FString firstline;

		UPROPERTY(EditAnywhere, Category = "line")
			FString secondline;

		UPROPERTY(EditAnywhere, Category = "line")
			FString thirdline;

		UPROPERTY(EditAnywhere, Category = "line")
			FString charnamestr;

		UPROPERTY(EditAnywhere, Category = "line")
			FString charsaystr;

		UPROPERTY(EditAnywhere, Category = "line")
			int charprogvar;


		

		//UPROPERTY(EditAnywhere, Category = "morph")
		//	UMaterialInstanceDynamic* morphmat;

		UPROPERTY(EditAnywhere, Category = "morph")
			UMaterialInterface* Material;

		UPROPERTY(EditAnywhere, Category = "morph")
			USkeletalMeshComponent* mymesh;

		UPROPERTY(EditAnywhere, Category = "morph")
		UMaterialInstanceDynamic* DynMaterial;

		UPROPERTY(EditAnywhere, Category = "morph")
			bool ismorphed;

		UPROPERTY(EditAnywhere, Category = "morph")
			bool isBeingCarried;

		UPROPERTY(EditAnywhere, Category = "morph")
			bool isRockbutOuttaWay;

		UPROPERTY(EditAnywhere, Category = "morph")
			bool HasBeenSeen;

		UPROPERTY(EditAnywhere, Category = "UI")
			bool XboxConOn;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	/*hud stuff*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widgets", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> wMainMenu;

	UPROPERTY()
		class UUserWidget* CurrentWidget;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

