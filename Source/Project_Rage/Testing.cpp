// Fill out your copyright notice in the Description page of Project Settings.


#include "Testing.h"

// Sets default values
ATesting::ATesting()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionMesh = CreateDefaultSubobject<class USphereComponent>(TEXT("CollisionMesh"));
	RootComponent = CollisionMesh; 
}

// Called when the game starts or when spawned
void ATesting::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATesting::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATesting::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

