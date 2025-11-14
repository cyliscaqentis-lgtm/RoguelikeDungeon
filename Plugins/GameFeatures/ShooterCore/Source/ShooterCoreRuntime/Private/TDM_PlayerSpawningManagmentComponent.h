// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"

#include "TDM_PlayerSpawningManagmentComponent.generated.h"

class AActor;
class AController;
class APlayerStart;
class UObject;

/**
 * 
 */
UCLASS()
class UTDM_PlayerSpawningManagmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UTDM_PlayerSpawningManagmentComponent(const FObjectInitializer& ObjectInitializer);

	AActor* OnChoosePlayerStart(AController* Player, TArray<APlayerStart*>& PlayerStarts);
	void OnFinishRestartPlayer(AController* Player, const FRotator& StartRotation);

protected:

};
