// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseGameMode.h"

ACPP_BaseGameMode::ACPP_BaseGameMode()
{
    //Obtains our player class ref and sets the default pawn of our gamemode to it
    static ConstructorHelpers::FClassFinder<APawn>
	PlayerRef(TEXT("Class'/Game/Blueprints/BP_Player.BP_Player_C'"));

	if(PlayerRef.Class)
	{
		DefaultPawnClass = PlayerRef.Class;
	}

    //Obtains our player controller and sets the player controller class to it 
    static ConstructorHelpers::FClassFinder<APlayerController>
    ControllerRef(TEXT("Class'/Game/Blueprints/BP_PlayerController.BP_PlayerController_C'"));

    if(ControllerRef.Class)
    {
        PlayerControllerClass = ControllerRef.Class; 
    }
}