#include "LyraTBSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Rogue/Turn/GameTurnManagerBase.h"
#include "UnitManagerSubsystem.h"

void ALyraTBSGameMode::StartPlay()
{
    Super::StartPlay();

    // クライアントでは生成しない（重複/競合防止）
    if (GetNetMode() == NM_Client) return;

    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameTurnManagerBase::StaticClass(), Found);
    if (Found.Num() == 0)
    {
        FActorSpawnParameters P;
        P.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        GetWorld()->SpawnActor<AGameTurnManagerBase>(
            AGameTurnManagerBase::StaticClass(),
            FVector::ZeroVector, FRotator::ZeroRotator, P
        );
    }
}

void ALyraTBSGameMode::TBS_DumpUnits()
{
    if (UUnitManagerSubsystem* UM = GetWorld()->GetSubsystem<UUnitManagerSubsystem>())
    {
        TArray<AActor*> Allies = UM->GetAlliesCopy();
        TArray<AActor*> Enemies = UM->GetEnemiesCopy();
        UE_LOG(LogTemp, Log, TEXT("=== TBS.DumpUnits ==="));
        UE_LOG(LogTemp, Log, TEXT("Allies: %d"), Allies.Num());
        for (AActor* A : Allies) { if (IsValid(A)) UE_LOG(LogTemp, Log, TEXT(" - %s"), *A->GetName()); }
        UE_LOG(LogTemp, Log, TEXT("Enemies: %d"), Enemies.Num());
        for (AActor* E : Enemies) { if (IsValid(E)) UE_LOG(LogTemp, Log, TEXT(" - %s"), *E->GetName()); }
    }
}
