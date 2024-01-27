// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerController.h"

void ACPP_PlayerController::OnPossess(APawn* PlayerPawn)
{
    Super::OnPossess(PlayerPawn); 

    //Obtains the players reference 
    PlayerRef = Cast<ACPP_Player>(PlayerPawn); 

    if(!PlayerRef)
    {
        UE_LOG(LogTemp, Fatal, TEXT("Player pawn failed cast to ACPP_Player2D type!"));
    }

    //Enables the default mapping controller context and links our IMC to it
    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

    if(Subsystem)
    {
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }
}

void ACPP_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    //Gives us access to the EnhancedInputComponent subsystem
    UEnhancedInputComponent*  EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent);

    if(EnhancedInputComp)
    {
        if(Move_Action)
        {
            //Binds the actions related to move action(left mouse click)
            EnhancedInputComp->BindAction(Move_Action, ETriggerEvent::Started, this, &ACPP_PlayerController::OnBeginMove);
            EnhancedInputComp->BindAction(Move_Action, ETriggerEvent::Ongoing, this, &ACPP_PlayerController::OnMoveBat);
            EnhancedInputComp->BindAction(Move_Action, ETriggerEvent::Completed, this, &ACPP_PlayerController::OnEndMove);
        }
    }
}

void ACPP_PlayerController::OnBeginMove()
{ 
    PlayerRef->BeginMove(); 
}

void ACPP_PlayerController::OnMoveBat()
{ 
    PlayerRef->MoveBat(); 
}

void ACPP_PlayerController::OnEndMove()
{ 
    PlayerRef->EndMove(); 
}