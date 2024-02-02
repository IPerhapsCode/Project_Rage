// Fill out your copyright notice in the Description page of Project Settings.
/* To Add
-A system to detach the camera from the player once he hits a certain trigger and then reatach it once that trigger is hit again
-Change the rotation rate in the air
-Could have a slopped block that stops the player as soon as it is touched 
-Could have a wind system with like a big timer in the background telling the player when the wind will start
-Add particules when the player is launched
-Maybe add an object that hits the player with an animation on player input, add camera shake on hit  
-Start making assets for the player and such 
-Need to tweek the displacement and rotational values
-Add sort of a double jump to the character
-Need to setup a git for this project
-There's possibly a bug with the way the speed of the bat is calculated where it is slower then intended but i cant currently replecate it consistently
-The multiplier is currently being changed based on resolution but it could be better 
*/
#include "CPP_Player.h"

//FIntPoint Resolution; wot

// Sets default values
ACPP_Player::ACPP_Player()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Creates a spherical collision mesh within the subobjects of our actor
	CollisionMesh = CreateDefaultSubobject<class USphereComponent>(TEXT("CollisionMesh"));
	CollisionMeshRadius = CollisionMesh->GetScaledSphereRadius(); 
	RootComponent = CollisionMesh;

	SpringArmCompFlipbook = CreateDefaultSubobject<USpringArmComponent>(TEXT("Player Flipbook Spring Arm")); 
	SpringArmCompFlipbook->SetupAttachment(RootComponent);
	SpringArmCompFlipbook->TargetArmLength = 0.f;
	SpringArmCompFlipbook->bInheritPitch = false;
	SpringArmCompFlipbook->bInheritYaw = false;
	SpringArmCompFlipbook->bInheritRoll = false;

	//Creates a flipbook within the subobjects of our actor, sets the flipbook to our default animation and attaches the subobject to CollisionMesh
	Flipbook = CreateDefaultSubobject<class UPaperFlipbookComponent>(TEXT("Flipbook")); 
	if(DebugFlipbook)
	{
		Flipbook->SetFlipbook(DebugFlipbook);
	}
	Flipbook->SetupAttachment(SpringArmCompFlipbook);

	//Attaches the springarm to our flipbook, changes the length of the springarm, and disables inherited rotation for the camera
	SpringArmCompCam = CreateDefaultSubobject<USpringArmComponent>(TEXT("Player Camera Spring Arm")); 
	SpringArmCompCam->SetupAttachment(Flipbook);
	SpringArmCompCam->TargetArmLength = 500.f; 
	SpringArmCompCam->bInheritPitch = false;
	SpringArmCompCam->bInheritYaw = false;
	SpringArmCompCam->bInheritRoll = false;
    
	//Attaches the camera to the springarm
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera")); 
	CameraComp->SetupAttachment(SpringArmCompCam);

	//Creates the bat flipbook subobject as well as attaching it to our springarmcomp flipbook
	Bat = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Bat"));
	if(DebugFlipbook)
	{
		Bat->SetFlipbook(DebugFlipbook); 
	}
	Bat->SetupAttachment(SpringArmCompFlipbook); 

	//Obtains the classes of every type of camera shake 
	static ConstructorHelpers::FClassFinder<class UCameraShakeBase>
	MoveWeakCameraShakeRef(TEXT("Class'/Game/Blueprints/CameraShakes/BP_MoveWeakCameraShake.BP_MoveWeakCameraShake_C'"));

	if(MoveWeakCameraShakeRef.Class)
	{
		MoveWeakCameraShake = MoveWeakCameraShakeRef.Class; 
	}

	static ConstructorHelpers::FClassFinder<class UCameraShakeBase>
	MoveMediumCameraShakeRef(TEXT("Class'/Game/Blueprints/CameraShakes/BP_MoveMediumCameraShake.BP_MoveMediumCameraShake_C'"));

	if(MoveMediumCameraShakeRef.Class)
	{
		MoveMediumCameraShake = MoveMediumCameraShakeRef.Class; 
	}

	static ConstructorHelpers::FClassFinder<class UCameraShakeBase>
	MoveStrongCameraShakeRef(TEXT("Class'/Game/Blueprints/CameraShakes/BP_MoveStrongCameraShake.BP_MoveStrongCameraShake_C'"));

	if(MoveStrongCameraShakeRef.Class)
	{
		MoveStrongCameraShake = MoveStrongCameraShakeRef.Class; 
	}
}

// Called when the game starts or when spawned
void ACPP_Player::BeginPlay()
{
	Super::BeginPlay();
	
	//Let's us obtain the player controller
	Input = GetController<APlayerController>();
	if(Input)
	{
		//Enables click and mouse over events
		Input->bEnableClickEvents = true; 
		Input->bEnableMouseOverEvents = true;
		Input->bShowMouseCursor = true;
	}

	//Let's us obtain the start x position of our actor 
	StartXActorPosition = GetActorLocation().X; 
	//Makes sure the bat is not visible once the game has started and no inputs have been pressed yet 
	Bat->SetVisibility(false);
	//Keeps the Y position of the bat so that it never leaves that position later on 
	BatYPosition = Bat->GetRelativeLocation().Y; 

	if(!CollisionMesh)
	{
		PRINT("Hello");
	}
}

// Called every frame
void ACPP_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ChangeRotation(); 

	StopPassiveMovement(DeltaTime); 

	//PRINT_VARS("X: %i, Y: %i", Green, Resolution.X, Resolution.Y);
}

// Called to bind functionality to input
void ACPP_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACPP_Player::BeginMove()
{
	if(DetectGround())
	{
		//Obtain the current viewport and determine the correct attack multiplier based on it (Probably can me optimized later to only do once if the resolution has been changed)
		int32 ViewportSize[2];
		Input->GetViewportSize(ViewportSize[0], ViewportSize[1]); //X, Y
		PRINT_VARS("X: %i, Y: %i", Red, ViewportSize[0], ViewportSize[1]);
		for(int32 i = 0; i < 2; ++i)
		{
			ImpulseMultiplier[i] = DefaultImpulseMultiplier[i] * DefaultViewport[i] / ViewportSize[i]; 
			BatMaxPosition[i] = MaxImpulse / ImpulseMultiplier[i]; 
		}
		PRINT_VARS("X: %g, Y: %g", Orange, ImpulseMultiplier[0], ImpulseMultiplier[1]);
		//Obtains our actors location and converts it to its screen location
		Input->ProjectWorldLocationToScreen(GetActorLocation(), StartPosition);
		//Sets our mouse position to the actors screen location 
		Input->SetMouseLocation(StartPosition.X, StartPosition.Y); 
		//Makes the bat visible (it replaced our cursor)
		Bat->SetRelativeLocation(FVector(0.f,BatYPosition,0.f)); 
		Bat->SetVisibility(true);
		FirstTickBatAttack = true;
		//Stops the camera shake if player begins a new move
		switch (LastCameraShake)
		{
		case 0:
			Input->ClientStopCameraShake(MoveWeakCameraShake);  
			break;
		case 1:
			Input->ClientStopCameraShake(MoveMediumCameraShake);
			break;
		case 2:
			Input->ClientStopCameraShake(MoveStrongCameraShake);
			break;
		case 3:
			break;
		}

		PRINT_VARS("StartPosition:%g %g",Green, StartPosition.X, StartPosition.Y);
		PRINT_VARS("ElapsedTime: %g", Blue, MoveStrongCameraShake.GetDefaultObject()->GetCameraShakeDuration().Get());
	}
}

void ACPP_Player::EndMove()
{
	//Obtains the position of the mouse cursor
	Input->GetMousePosition(EndPosition.X, EndPosition.Y);  
	//Starts the logic for the bat attack animation and the impulse logic
	if(DetectGround())
	{
		GetWorldTimerManager().SetTimer(BatTimer, this, &ACPP_Player::BatAttack, TimerRefreshRate, true); 
	}
	else
	{
		EndMoveLogic(); 
	}
}

//Makes sure that the impulse does not go beyond it's maximum possible value
float ACPP_Player::StandardizedInput(float Impulse, float Max)
{
	if(Impulse >= 0)
	{
		return FMath::Min(Impulse, Max);
	}
	else
	{
		return FMath::Max(Impulse, -Max);
	}
}

//Let's us change the rotation of our flipbook at our own rate
void ACPP_Player::ChangeRotation()
{
	//Determines the new rotation based on the actors position relative to the origin point
	NewRotation.Pitch = ((GetActorLocation().X - StartXActorPosition)/360.f) * - RotationMultiplier; 
	//Sets the new rotation of our flipbook
	Flipbook->SetRelativeRotation(NewRotation);
}

//Lets us detect if the player is standing on ground
bool ACPP_Player::DetectGround()
{
	FVector Start = GetActorLocation(); 
	Start = FVector(Start.X, Start.Y - CollisionMeshRadius - 1, Start.Z); 
	FVector End = FVector(Start.X, Start.Y, Start.Z - CollisionMeshRadius - 15);
	FHitResult Hit; 
	bool OnGround = false;

	if(GetWorld())
	{
		OnGround = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_PhysicsBody, FCollisionQueryParams(), FCollisionResponseParams());
		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.01f, 0.f, 10.f); 
	}

	return OnGround;
}

//Lets us stop the slow rotation the player has after a move
void ACPP_Player::StopPassiveMovement(float DeltaTime)
{
	FVector Velocity = CollisionMesh->GetComponentVelocity();
	float NewVelocityX; 

	if(Velocity.X < PassiveMovementDecreaseRoof && Velocity.X > 0 && Velocity.Z < 0.01)
	{
		NewVelocityX = FMath::Max(0, Velocity.X - PassiveMovementDecreaseRate * DeltaTime); 
		CollisionMesh->SetPhysicsLinearVelocity(FVector(NewVelocityX, Velocity.Y, Velocity.Z)); 
	}
	else if(Velocity.X > -PassiveMovementDecreaseRoof && Velocity.X < 0 && Velocity.Z < 0.01)
	{
		NewVelocityX = FMath::Min(0, Velocity.X + PassiveMovementDecreaseRate * DeltaTime); 
		CollisionMesh->SetPhysicsLinearVelocity(FVector(NewVelocityX, Velocity.Y, Velocity.Z));  
	}
}

void ACPP_Player::MoveBat()
{
	if(Bat->IsVisible())
	{
		FVector CurrentPosition;
		FVector PlayerPosition;
		float XValue;
		float ZValue; 

		//Obtains the location of the mouse in the world as well the location of our actor 
		Input->DeprojectMousePositionToWorld(CurrentPosition, PlayerPosition);
		PlayerPosition = GetActorLocation();

		//Offsets the X and Z value as well as increasing its displacement speed
		XValue = (CurrentPosition.X - PlayerPosition.X) * 44;
		ZValue = (CurrentPosition.Z - PlayerPosition.Z) * 44 + 40;

		//Makes sure the character doesnt jiggle when he gets to the edge of the zone
		FVector XMaxValue;
		FVector ZMaxValue; 
		FVector Temp;

		//X axis limit
		if(XValue <= 0)
		{
			//Converts the screen position of the max mouse position to the relative position of the bat
			Input->DeprojectScreenPositionToWorld(StartPosition.X - BatMaxPosition[0], StartPosition.Y, XMaxValue, Temp);
			//Takes the value that is within the limits
			XValue = FMath::Max((XMaxValue.X - PlayerPosition.X) * 44, XValue); 
		}
		else
		{
			//Converts the screen position of the max mouse position to the relative position of the bat
			Input->DeprojectScreenPositionToWorld(StartPosition.X + BatMaxPosition[0], StartPosition.Y, XMaxValue, Temp);
			//Takes the value that is within the limits
			XValue = FMath::Min((XMaxValue.X - PlayerPosition.X) * 44, XValue); 
		}

		//Z axis limit
		if(ZValue <= 0)
		{
			//Converts the screen position of the max mouse position to the relative position of the bat
			Input->DeprojectScreenPositionToWorld(StartPosition.X, StartPosition.Y + BatMaxPosition[1], ZMaxValue, Temp); 
			//Takes the value that is within the limits
			ZValue = FMath::Max((ZMaxValue.Z - PlayerPosition.Z) * 44 + 40, ZValue);
		}
		else
		{
			//Converts the screen position of the max mouse position to the relative position of the bat
			Input->DeprojectScreenPositionToWorld(StartPosition.X, StartPosition.Y - BatMaxPosition[1], ZMaxValue, Temp);
			//Takes the value that is within the limits
			ZValue = FMath::Min((ZMaxValue.Z - PlayerPosition.Z) * 44 + 40, ZValue);
		}

		//Sets the relative location of the bat 
		Bat->SetRelativeLocation(FVector(XValue, BatYPosition, ZValue));

		MaxBatMovement(); 
	}
}

//Creates a square limit around the bat, there's no point to going outside said limit as max power will be reached within it, is not perfect imo 
void ACPP_Player::MaxBatMovement()
{
	FVector2D MousePosition;
	
	Input->GetMousePosition(MousePosition.X, MousePosition.Y); 

	if(MousePosition.X > StartPosition.X + BatMaxPosition[0])
	{
		
		MousePosition.X = StartPosition.X + BatMaxPosition[0];
	}
	else if(MousePosition.X < StartPosition.X - BatMaxPosition[0])
	{
		MousePosition.X = StartPosition.X - BatMaxPosition[0];
	}

	if(MousePosition.Y > StartPosition.Y + BatMaxPosition[1])
	{
		MousePosition.Y = StartPosition.Y + BatMaxPosition[1];
	}
	else if(MousePosition.Y < StartPosition.Y - BatMaxPosition[1])
	{
		MousePosition.Y = StartPosition.Y - BatMaxPosition[1]; 
	}

	Input->SetMouseLocation(MousePosition.X, MousePosition.Y); 

	//Hypothetical version where the mouse is moved instead of the bat doesnt quite work circle limit 
	// FVector2D MousePosition;
	// FVector2D NewPosition;

	// Input->GetMousePosition(MousePosition.X, MousePosition.Y); 

	// NewPosition.X = FMath::Sqrt(abs(FMath::Pow(RayonMaxBat, 2) - FMath::Pow(MousePosition.Y - StartPosition.Y, 2)));
	// NewPosition.Y = FMath::Sqrt(abs(FMath::Pow(RayonMaxBat, 2) - FMath::Pow(MousePosition.X - StartPosition.X, 2))); 

	// if(MousePosition.Y > NewPosition.Y + StartPosition.Y && MousePosition.X < StartPosition.X + RayonMaxBat/1.5 && MousePosition.X > StartPosition.X - RayonMaxBat/1.5)
	// {
	// 	MousePosition.Y = NewPosition.Y + StartPosition.Y;
	// }
	// else if(MousePosition.Y < -NewPosition.Y + StartPosition.Y && MousePosition.X < StartPosition.X + RayonMaxBat/1.5 && MousePosition.X > StartPosition.X - RayonMaxBat/1.5)
	// {
	// 	MousePosition.Y = -NewPosition.Y + StartPosition.Y; 
	// }
	// else if(MousePosition.X > NewPosition.X + StartPosition.X && MousePosition.X > StartPosition.X + RayonMaxBat - RayonMaxBat/1.5)
	// {
	// 	MousePosition.X = NewPosition.X + StartPosition.X; 
	// }
	// else if(MousePosition.X < -NewPosition.X + StartPosition.X && MousePosition.X < StartPosition.X - RayonMaxBat + RayonMaxBat/1.5)
	// {
	// 	MousePosition.X = -NewPosition.X + StartPosition.X; 
	// }

	// Input->SetMouseLocation(MousePosition.X, MousePosition.Y); 
}

//Faque, je l'ai codé pour utiliser une fonction exponentielle, finalement je m'en compte qu'une simple fonction linéaire serait mieux pour faire l'animation de déplacement de la bat :(
void ACPP_Player::BatAttack()
{
	if(FirstTickBatAttack)
	{
		FirstTickBatAttack = false;
		BatRelPosition = Bat->GetRelativeLocation();
		ALinearBatAttack = BatRelPosition.Z / BatRelPosition.X; 
		
		//Makes sure that wherever the player is batting from, the animation still takes the same amount of time
	 	FVector2D MousePosition; 
	 	Input->GetMousePosition(MousePosition.X, MousePosition.Y);
		SpeedBatAttack = DefaultSpeedBatAttack * abs(MousePosition.X - StartPosition.X) / BatMaxPosition[0];
	}

	BatRelPosition.Z = ALinearBatAttack * BatRelPosition.X; 
	
	if(BatRelPosition.X >= 0)
	{ 
		BatRelPosition.X -= SpeedBatAttack * FApp::GetDeltaTime();
		if(BatRelPosition.X <= 0 )
		{ 
			EndMoveLogic(); 
		} 
	}
	else if(BatRelPosition.X < 0)
	{
		BatRelPosition.X += SpeedBatAttack * FApp::GetDeltaTime();
		
		if(BatRelPosition.X >= -0.3 ) //Needs to be -0.3 to fail safe if the player doesnt move the bat 
		{ 
			EndMoveLogic(); 
		}  
	}

	Bat->SetRelativeLocation(FVector(BatRelPosition.X, BatYPosition, BatRelPosition.Z)); 

	//Im keeping the following code commented cuz fuck me I worked hard on it but fuck is it useless
	//Initialises the values to their necessary values for the exponential curve movement
	// if(FirstTickBatAttack)
	// {
	// 	FirstTickBatAttack = false;
	// 	BatRelPosition = Bat->GetRelativeLocation();
	// 	BExpoBatAttack = FMath::Pow(abs(BatRelPosition.Z)/AExpoBatAttack, 1/BatRelPosition.X);
	// 	XBatAttack = BatRelPosition.X; 
	// 	ZBatAttack = BatRelPosition.Z;

	// 	//Makes sure that wherever the player is batting from, the animation still takes the same amount of time
	// 	FVector2D MousePosition; 
	// 	Input->GetMousePosition(MousePosition.X, MousePosition.Y);
	// 	SpeedBatAttack = DefaultSpeedBatAttack * abs(MousePosition.X - StartPosition.X) / BatMaxPosition[0];
	// }

	// //Moves the bat according to an exponential curve, once X = 0 ends the movement and calls the impulse logic
	// if(EndPosition.Y <= StartPosition.Y)
	// {
	// 	ZBatAttack = AExpoBatAttack*FMath::Pow(BExpoBatAttack, XBatAttack); 
	// }
	// else if(EndPosition.Y > StartPosition.Y)
	// {
	// 	ZBatAttack = -AExpoBatAttack*FMath::Pow(BExpoBatAttack, XBatAttack); 
	// }

	// Bat->SetRelativeLocation(FVector(XBatAttack, BatYPosition, ZBatAttack)); 
	// if(XBatAttack <= 0)
	// {
	// 	XBatAttack += SpeedBatAttack * FApp::GetDeltaTime();
	// 	if(XBatAttack >= 0)
	// 	{
	// 		EndMoveLogic();
			
	// 	}
	// }
	// else if(XBatAttack > 0)
	// {
	// 	XBatAttack -= SpeedBatAttack * DeltaTime;
	// 	if(XBatAttack <= 0)
	// 	{
	// 		EndMoveLogic();
	// 	}
	// }
}

void ACPP_Player::EndMoveLogic()
{
	GetWorldTimerManager().ClearTimer(BatTimer);

	if(DetectGround() && Bat->IsVisible())
	{ 
		PRINT_VARS("EndPosition:%g %g",Red, EndPosition.X, EndPosition.Y);
		//Applies an impulse to our actor from its mesh 
		XImpulse = StandardizedInput((StartPosition.X - EndPosition.X) * ImpulseMultiplier[0], MaxImpulse);
		YImpulse = StandardizedInput((EndPosition.Y - StartPosition.Y) * ImpulseMultiplier[1], MaxImpulse);
		PRINT_VARS("XImpulse:%g, YImpulse %g",Blue, XImpulse, YImpulse);
		CollisionMesh->AddImpulse(FVector(XImpulse,0,YImpulse), FName(TEXT("None")), true); 
		
		if(abs(XImpulse) + abs(YImpulse) < 500)
		{
			Input->ClientStartCameraShake(MoveWeakCameraShake);
			LastCameraShake = Weak;
		}
		else if(abs(XImpulse) + abs(YImpulse) < 1000)
		{
			Input->ClientStartCameraShake(MoveMediumCameraShake);
			LastCameraShake = Medium;
		}
		else
		{
			Input->ClientStartCameraShake(MoveStrongCameraShake);
			LastCameraShake = Strong;
		}
	}

	//Resets our variables values
	StartPosition = FVector2D();
	EndPosition = FVector2D();
	XImpulse = 0.f;
	YImpulse = 0.f; 
	//Hides the players bat (replaces the cursor)
	Bat->SetVisibility(false);
	//Places the bat at the center of the player for next movement input 
	Bat->SetRelativeLocation(FVector(0.f,BatYPosition,0.f)); 
}

// FVector2D ACPP_Player::GetGameResolution()
// {
// 	// FVector2D Results;

// 	// Results.X = GSystemResolution.ResX;
// 	// Results.Y = GSystemResolution.ResY;

// 	// return Results; 

// 	FVector2D Result = FVector2D( 1, 1 );

// 	if ( GEngine && GEngine->GameViewport )
// 	{
// 		GEngine->GameViewport->GetViewportSize( Result );
// 	}

// 	return Result;
// }



