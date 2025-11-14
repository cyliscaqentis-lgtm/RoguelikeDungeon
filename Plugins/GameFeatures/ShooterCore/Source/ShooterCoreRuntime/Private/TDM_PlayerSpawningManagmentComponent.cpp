// Copyright Epic Games, Inc. All Rights Reserved.

#include "TDM_PlayerSpawningManagmentComponent.h"

#include "Engine/World.h"
#include "GameFramework/PlayerState.h"
#include "GameModes/LyraGameState.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "Teams/LyraTeamSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TDM_PlayerSpawningManagmentComponent)

class AActor;

UTDM_PlayerSpawningManagmentComponent::UTDM_PlayerSpawningManagmentComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

AActor* UTDM_PlayerSpawningManagmentComponent::OnChoosePlayerStart(AController* Player, TArray<APlayerStart*>& PlayerStarts)
{
	ULyraTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<ULyraTeamSubsystem>();
	if (!ensure(TeamSubsystem))
	{
		return nullptr;
	}

	const int32 PlayerTeamId = TeamSubsystem->FindTeamFromObject(Player);

	// We should have a TeamId by now, but early login stuff before post login can try to do stuff, ignore it.
	if (!ensure(PlayerTeamId != INDEX_NONE))
	{
		return nullptr;
	}

	ALyraGameState* GameState = GetWorld() != nullptr ? GetWorld()->GetGameState<ALyraGameState>() : nullptr;

	APlayerStart* BestPlayerStart = nullptr;
	double MaxDistance = 0;

	for (APlayerState* PS : GameState->PlayerArray)
	{
		const int32 TeamId = TeamSubsystem->FindTeamFromObject(PS);
		
		// We should have a TeamId by now...
		if (PS->IsOnlyASpectator() || !ensure(TeamId != INDEX_NONE))
		{
			continue;
		}

		// If the other player isn't on the same team, lets find the furthest spawn from them.
		if (TeamId != PlayerTeamId)
		{
			for (APlayerStart* PlayerStart : PlayerStarts)
			{
				if (APawn* Pawn = PS->GetPawn())
				{
					const double Distance = PlayerStart->GetDistanceTo(Pawn);

					// Simplified logic - just find the furthest spawn
					if (BestPlayerStart == nullptr || Distance > MaxDistance)
					{
						BestPlayerStart = PlayerStart;
						MaxDistance = Distance;
					}
				}
			}
		}
	}

	return BestPlayerStart;
}

void UTDM_PlayerSpawningManagmentComponent::OnFinishRestartPlayer(AController* Player, const FRotator& StartRotation)
{
	
}
