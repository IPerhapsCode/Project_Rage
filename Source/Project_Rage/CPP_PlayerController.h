// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#pragma region My Includes
#include "CPP_Player.h"
#include "Project_Rage.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#pragma endregion My Includes

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPP_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_RAGE_API ACPP_PlayerController : public APlayerController
{
	GENERATED_BODY()
	

	#pragma region Controller Variables
	public:
	protected:
	private:
		//Reference to our IMC
		UPROPERTY(EditDefaultsOnly, Category="Player | Input")
		TObjectPtr<class UInputMappingContext> DefaultMappingContext = nullptr;
		
		//Reference to IA
		UPROPERTY(EditDefaultsOnly, Category="Player | Input")
		TObjectPtr<class UInputAction> Move_Action = nullptr; 
		
		//Player reference
		UPROPERTY()
		TObjectPtr<class ACPP_Player> PlayerRef = nullptr;
	#pragma endregion Controller Variables

	#pragma region Controller Functions
	public:
	protected:
	private:
		void OnPossess(APawn* PlayerPawn) override;

		void SetupInputComponent() override;

		void OnBeginMove(); 

		void OnMoveBat(); 

		void OnEndMove();
	#pragma endregion Controller Functions
};
