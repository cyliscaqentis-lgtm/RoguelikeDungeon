#include "LyraPlayerState_TBS.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"

void ALyraPlayerState_TBS::SetPawnData_BP(const ULyraPawnData* InPawnData)
{
    if (InPawnData == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("SetPawnData_BP called with null PawnData"));
        return;
    }

    SetPawnData(InPawnData);

    UE_LOG(LogTemp, Log, TEXT("SetPawnData_BP executed: %s"), *InPawnData->GetName());
}
void ALyraPlayerState_TBS::InitASCActorInfo_BP(APawn* AvatarPawn)
{
    if (auto* ASC = GetLyraAbilitySystemComponent())
    {
        ASC->InitAbilityActorInfo(this, AvatarPawn);
        UE_LOG(LogTemp, Log, TEXT("ASC manually initialized: Owner=%s Avatar=%s"),
            *GetNameSafe(this), *GetNameSafe(AvatarPawn));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("InitASCActorInfo_BP failed: No ASC on %s"), *GetNameSafe(this));
    }
}
