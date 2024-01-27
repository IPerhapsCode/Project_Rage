// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#pragma region My Includes

#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperCharacter.h"
#include "Engine/EngineTypes.h"
#include "Components/BoxComponent.h"
#include "Project_Rage.h"
#include "string.h"

#pragma endregion My Includes

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_Platform.generated.h"

UCLASS()
class PROJECT_RAGE_API ACPP_Platform : public AActor
{
	GENERATED_BODY()

	#pragma region Variables

	UPROPERTY(VisibleAnywhere, Category="Platform | Flipbook")
	TObjectPtr<class UPaperFlipbookComponent> Flipbook = nullptr;

	UPROPERTY(EditAnywhere, Category="Platform | Flipbook") 
	TObjectPtr<class UPaperFlipbook> DebugFlipbook = nullptr;

	UPROPERTY(EditAnywhere, Category="Platform | Flipbook") 
	TObjectPtr<class UPaperSpriteComponent> Test;

	UPROPERTY(EditAnywhere, Category="Platform | Flipbook") 
	TObjectPtr<class UPaperSpriteComponent> Test2;

	UPROPERTY( EditAnywhere, Category="Platform", meta = ( UseComponentPicker ) )
	FComponentReference testing; //Holy shit comment tu fonction tu fais aucun sens wtffffffff

	UPROPERTY(VisibleAnywhere, Category="Platform")
	TObjectPtr<class UBoxComponent> CollisionMesh = nullptr;

	#pragma endregion Variables

	#pragma region Functions
	#pragma endregion Functions
	
public:	
	// Sets default values for this actor's properties
	ACPP_Platform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
