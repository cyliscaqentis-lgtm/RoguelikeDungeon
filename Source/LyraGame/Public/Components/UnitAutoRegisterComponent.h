#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnitAutoRegisterComponent.generated.h"

UCLASS(ClassGroup=(TBS), meta=(BlueprintSpawnableComponent))
class LYRAGAME_API UUnitAutoRegisterComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    // true=味方 / false=敵
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Unit")
    bool bIsAlly = true;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
