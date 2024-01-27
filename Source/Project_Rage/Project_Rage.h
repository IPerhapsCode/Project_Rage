// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#pragma region Macros
#define PRINT(Message) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::White, TEXT(Message))
#define PRINT_VARS(Message, Color, ...) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Color, FString::Printf(TEXT(Message), ##__VA_ARGS__))
#define PRINT_BOOL(Message, Color, Bool) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Color, FString::Printf(TEXT(Message), Bool?TEXT("True"):TEXT("False")))
#pragma endregion Macros

