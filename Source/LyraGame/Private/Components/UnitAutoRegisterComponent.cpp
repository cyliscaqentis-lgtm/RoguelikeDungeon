#include "Components/UnitAutoRegisterComponent.h"
#include "UnitManagerSubsystem.h"

void UUnitAutoRegisterComponent::BeginPlay()
{
    Super::BeginPlay();
    if (UWorld* W = GetWorld())
    {
        if (UUnitManagerSubsystem* UM = W->GetSubsystem<UUnitManagerSubsystem>())
        {
            UM->RegisterUnit(GetOwner(), bIsAlly);
        }
    }
}

void UUnitAutoRegisterComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (UWorld* W = GetWorld())
    {
        if (UUnitManagerSubsystem* UM = W->GetSubsystem<UUnitManagerSubsystem>())
        {
            UM->UnregisterUnit(GetOwner());
        }
    }
    Super::EndPlay(EndPlayReason);
}
