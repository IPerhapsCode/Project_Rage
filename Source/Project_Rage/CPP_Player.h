// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#pragma region My Includes
#include "Project_Rage.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperCharacter.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Misc/App.h" 
#pragma endregion My Includes

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CPP_Player.generated.h"

UENUM()
enum PossibleCameraShakes {Weak, Medium, Strong, Undefined};

UCLASS()
class PROJECT_RAGE_API ACPP_Player : public APawn
{
	GENERATED_BODY()

	#pragma region Member Variables
	public:
		//Creates a springarm subobject within our actor
		UPROPERTY(VisibleAnywhere, Category="Player | Camera")
		TObjectPtr<class USpringArmComponent> SpringArmCompCam;

		//Creates a camera subobject within our actor
		UPROPERTY(VisibleAnywhere, Category="Player | Camera")
		TObjectPtr<class UCameraComponent> CameraComp;

		//Reference to our player controller 
		UPROPERTY()
		TObjectPtr<class APlayerController> Input = nullptr; 

		//Start position of the mouse when beginning movement
		UPROPERTY(VisibleAnywhere, Category="Player | Movement")
		FVector2D StartPosition = FVector2D();

		//End position of the mouse when ending movement 
		UPROPERTY(VisibleAnywhere, Category="Player | Movement")
		FVector2D EndPosition = FVector2D();
		
		//Reference to our bat flipbook component
		UPROPERTY(VisibleAnywhere, Category="Player | Animation")
		TObjectPtr<class UPaperFlipbookComponent> Bat; 

		//Relative Y position of the bat to the player 
		UPROPERTY(VisibleAnywhere, Category="Player | Animation")
		float BatYPosition; 

		//Reference to our player flipbook component
		UPROPERTY(VisibleAnywhere, Category="Player | Animation")
		TObjectPtr<class UPaperFlipbookComponent> Flipbook;

		UPROPERTY(VisibleAnywhere, Category="Player | Camera")
		TObjectPtr<class USpringArmComponent> SpringArmCompFlipbook;

		//Reference to the debug flipbook asset
		UPROPERTY(EditDefaultsOnly, Category="Player | Animation")
		TObjectPtr<class UPaperFlipbook> DebugFlipbook; 

		//Reference to our collision mesh 
		UPROPERTY(EditDefaultsOnly, Category="Player | Movement")
		TObjectPtr<class USphereComponent> CollisionMesh; 

		UPROPERTY(VisibleAnywhere, Category="Player | Movement")
		float CollisionMeshRadius; 

		//X axis impulse strength
		UPROPERTY(VisibleAnywhere, Category="Player | Movement")
		float XImpulse = 0.f;  

		//Y axis impulse strength
		UPROPERTY(VisibleAnywhere, Category="Player | Movement")
		float YImpulse = 0.f; 

		//X axis impulse multiplier
		UPROPERTY(EditAnywhere, Category="Player | Movement")
		float ImpulseMultiplier = 2.f;

		//X axis max impulse strength
		UPROPERTY(EditAnywhere, Category="Player | Movement")
		float MaxImpulse = 1000.f;  

		//Determines if we should initialise data in the bat attack 
		UPROPERTY(VisibleAnywhere, Category="Player | Movement | BatAttack")
		bool FirstTickBatAttack = true;

		//Relative position of the bat
		UPROPERTY(VisibleAnywhere, Category="Player | Movement | BatAttack")
		FVector BatRelPosition; 

		//Max speed of the animation 
		UPROPERTY(EditAnywhere, Category="Player | Movement | BatAttack")
		float DefaultSpeedBatAttack = 1500; 
		
		//Determines the speed of the animation
		UPROPERTY(VisibleAnywhere, Category="Player | Movement | BatAttack")
		float SpeedBatAttack = DefaultSpeedBatAttack; 

		UPROPERTY(EditAnywhere, Category="Player | Movement | BatAttack")
		float ALinearBatAttack;

		UPROPERTY(VisibleAnywhere, Category="Player | Movement | BatAttack")
		FTimerHandle BatTimer;

		UPROPERTY(VisibleAnywhere, Category="Player")
		float TimerRefreshRate = 0.01f;

		// UPROPERTY(EditAnywhere, Category="Player | Movement | Bat Attack")
		// float AExpoBatAttack = 0.2; 

		// UPROPERTY(VisibleAnywhere, Category="Player | Movement | Bat Attack")
		// float BExpoBatAttack;

		// UPROPERTY(VisibleAnywhere, Category="Player | Movement | Bat Attack")
		// float XBatAttack; 

		// UPROPERTY(VisibleAnywhere, Category="Player | Movement | Bat Attack")
		// float ZBatAttack;  

		//Start position on the x axis of the actor 
		UPROPERTY(VisibleAnywhere, Category="Player | Rotation")
		float StartXActorPosition = 0.f;
		
		//Determines the new rotation of our flipbook
		UPROPERTY(VisibleAnywhere, Category="Player | Rotation")
		FRotator NewRotation = FRotator(0,0,0);

		UPROPERTY(EditAnywhere, Category="Player | Rotation")
		float PassiveMovementDecreaseRate = 15.f; 

		UPROPERTY(EditAnywhere, Category="Player | Rotation")
		float PassiveMovementDecreaseRoof = 300.f; 

		//Determines how fast the flipbook will rotate
		UPROPERTY(EditAnywhere, Category="Player | Rotation")
		float RotationMultiplier = 50.f;

		UPROPERTY(VisibleAnywhere, Category="Player | Movement")
		float BatMaxPosition = MaxImpulse / ImpulseMultiplier;

		UPROPERTY(EditDefaultsOnly, Category="Player | Movement | BatAttack")
		TSubclassOf<UCameraShakeBase> MoveWeakCameraShake; 

		UPROPERTY(EditDefaultsOnly, Category="Player | Movement | BatAttack")
		TSubclassOf<UCameraShakeBase> MoveMediumCameraShake; 

		UPROPERTY(EditDefaultsOnly, Category="Player | Movement | BatAttack")
		TSubclassOf<UCameraShakeBase> MoveStrongCameraShake; 

		UPROPERTY(VisibleAnywhere, Category="Player | Movement | BatAttack")
		TEnumAsByte<PossibleCameraShakes> LastCameraShake = Undefined;

		
	protected:
	private:
	#pragma endregion Member Variables

	#pragma region Member Functions
	public:
		bool DetectGround(); 

		void BeginMove(); 

		void EndMove();

		void EndMoveLogic();  

		float StandardizedInput(float Impulse, float Max);

		void ChangeRotation(); 

		void StopPassiveMovement(float DeltaTime); 

		void MoveBat(); 

		void MaxBatMovement();

		void BatAttack(); 
	protected:
	private:
	#pragma endregion Member Functions

public:
	// Sets default values for this pawn's properties
	ACPP_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
