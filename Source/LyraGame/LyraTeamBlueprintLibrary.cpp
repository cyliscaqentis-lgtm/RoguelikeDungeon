#include "LyraTeamBlueprintLibrary.h"
#include "Teams/LyraTeamSubsystem.h"
#include "Teams/LyraTeamAgentInterface.h"
#include "GameFramework/PlayerState.h"
#include "Engine/World.h"

void ULyraTeamBlueprintLibrary::ChangeTeamForActor(AActor* Actor, int32 NewTeamId)
{
	if (!Actor)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ChangeTeamForActor] Invalid Actor"));
		return;
	}

	UWorld* World = Actor->GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ChangeTeamForActor] No World Context"));
		return;
	}

	if (ULyraTeamSubsystem* TeamSubsystem = World->GetSubsystem<ULyraTeamSubsystem>())
	{
		TeamSubsystem->ChangeTeamForActor(Actor, FGenericTeamId(NewTeamId));
		UE_LOG(LogTemp, Log, TEXT("[ChangeTeamForActor] %s assigned to Team %d"),
			*Actor->GetName(), NewTeamId);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ChangeTeamForActor] TeamSubsystem not found"));
	}
}
