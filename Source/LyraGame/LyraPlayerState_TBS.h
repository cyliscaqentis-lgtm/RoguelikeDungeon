#pragma once

#include "CoreMinimal.h"
#include "Player/LyraPlayerState.h"      // ← あなたの構造に合わせた include
#include "Character/LyraPawnData.h"      // ← 同上
#include "LyraPlayerState_TBS.generated.h"

/**
 *  BlueprintからSetPawnDataを呼べるようにした派生クラス
 */
UCLASS()
class LYRAGAME_API ALyraPlayerState_TBS : public ALyraPlayerState
{
    GENERATED_BODY()

public:

    /** Blueprintから呼べるSetPawnDataラッパ */
    UFUNCTION(BlueprintCallable, Category = "Lyra|Pawn")
    void SetPawnData_BP(const ULyraPawnData* InPawnData);
    UFUNCTION(BlueprintCallable, Category = "Lyra|AbilitySystem")
    void InitASCActorInfo_BP(APawn* AvatarPawn);

};
