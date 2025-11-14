#pragma once

#include "CoreMinimal.h"
// Lyra の GameMode 親。環境によっては "GameModes/LyraGameMode.h" パスです。
#include "GameModes/LyraGameMode.h"
#include "LyraTBSGameMode.generated.h"

UCLASS()
class LYRAGAME_API ALyraTBSGameMode : public ALyraGameMode
{
    GENERATED_BODY()

protected:
    virtual void StartPlay() override;

public:
    // コンソール: TBS.DumpUnits
    UFUNCTION(Exec)
    void TBS_DumpUnits();
};
