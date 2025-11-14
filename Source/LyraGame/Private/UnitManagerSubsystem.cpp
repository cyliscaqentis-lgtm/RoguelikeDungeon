#include "UnitManagerSubsystem.h"
#include "Engine/World.h"

UUnitManagerSubsystem* UUnitManagerSubsystem::Get(const UObject* WorldContextObject)
{
    if (!WorldContextObject) return nullptr;
    if (const UWorld* World = WorldContextObject->GetWorld())
    {
        return World->GetSubsystem<UUnitManagerSubsystem>();
    }
    return nullptr;
}

void UUnitManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    Allies.Reset();
    Enemies.Reset();
}

void UUnitManagerSubsystem::Deinitialize()
{
    Allies.Reset();
    Enemies.Reset();
    Super::Deinitialize();
}

void UUnitManagerSubsystem::RegisterUnit(AActor* Unit, bool bIsAlly)
{
    if (!IsValid(Unit)) return;
    (bIsAlly ? Allies : Enemies).AddUnique(Unit);
}

void UUnitManagerSubsystem::UnregisterUnit(AActor* Unit)
{
    Allies.Remove(Unit);
    Enemies.Remove(Unit);
}

void UUnitManagerSubsystem::GetAllies(TArray<AActor*>& OutAllies) const
{
    OutAllies.Reset();
    for (const TWeakObjectPtr<AActor>& WeakActor : Allies)
    {
        if (AActor* Actor = WeakActor.Get())
        {
            OutAllies.Add(Actor);
        }
    }
}

void UUnitManagerSubsystem::GetEnemies(TArray<AActor*>& OutEnemies) const
{
    OutEnemies.Reset();
    for (const TWeakObjectPtr<AActor>& WeakActor : Enemies)
    {
        if (AActor* Actor = WeakActor.Get())
        {
            OutEnemies.Add(Actor);
        }
    }
}

TArray<AActor*> UUnitManagerSubsystem::GetAlliesCopy() const
{
    TArray<AActor*> Out;
    Out.Reserve(Allies.Num());
    for (const TWeakObjectPtr<AActor>& It : Allies)
        if (It.IsValid()) Out.Add(It.Get());
    return Out;
}

TArray<AActor*> UUnitManagerSubsystem::GetEnemiesCopy() const
{
    TArray<AActor*> Out;
    Out.Reserve(Enemies.Num());
    for (const TWeakObjectPtr<AActor>& It : Enemies)
        if (It.IsValid()) Out.Add(It.Get());
    return Out;
}
