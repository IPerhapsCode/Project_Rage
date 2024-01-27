// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/SphereComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Testing.generated.h"

UCLASS()
class PROJECT_RAGE_API ATesting : public ACharacter
{
	GENERATED_BODY()

	public:
		UPROPERTY(EditDefaultsOnly, Category="Player | Animation")
		TObjectPtr<class USphereComponent> CollisionMesh; 

public:
	// Sets default values for this character's properties
	ATesting();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
