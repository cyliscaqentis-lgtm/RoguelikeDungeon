#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "LyraTeamBlueprintLibrary.generated.h"

UCLASS()
class LYRAGAME_API ULyraTeamBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Blueprint”Å ChangeTeamForActor */
	UFUNCTION(BlueprintCallable, Category = "Lyra|Team")
	static void ChangeTeamForActor(AActor* Actor, int32 NewTeamId);
};
