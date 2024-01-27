// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Platform.h"

// Sets default values
ACPP_Platform::ACPP_Platform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	CollisionMesh = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision Mesh"));
	RootComponent = CollisionMesh; 

	Flipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook")); 
	Flipbook->SetupAttachment(CollisionMesh); 
	if(DebugFlipbook)
	{
		Flipbook->SetFlipbook(DebugFlipbook); 
	}

	// Test = GetOwner()->FindComponentByClass<UPaperSpriteComponent>();
	// FComponentReference testing = FComponentReference();
	// testing.ComponentProperty = TEXT("allo"); 
	// Test = Cast<UPaperSpriteComponent>(testing.GetComponent(this));
}

// Called when the game starts or when spawned
void ACPP_Platform::BeginPlay()
{
	Super::BeginPlay();
	// Test = FindComponentByClass<UPaperSpriteComponent>(); 
	// Test2 = Cast<UPaperSpriteComponent>(FindComponentByTag(UPaperSpriteComponent::StaticClass(), FName("allo"))); 
	// PRINT_VARS("Test: %s", Green, *Test.GetFullName());
	// PRINT_VARS("Test2: %s", Red, *Test2.GetFullName());
}

// Called every frame
void ACPP_Platform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

