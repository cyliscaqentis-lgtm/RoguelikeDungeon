#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "UnitManagerSubsystem.generated.h"

/**
 * Unit 管理を Blueprint 配置の Actor から WorldSubsystem に移行。
 * 旧 BP の公開関数に合わせて最低限の互換 API を用意。
 */
UCLASS()
class LYRAGAME_API UUnitManagerSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    // BP から取得しやすい静的アクセサ
    UFUNCTION(BlueprintPure, Category="UnitManager", meta=(WorldContext="WorldContextObject"))
    static UUnitManagerSubsystem* Get(const UObject* WorldContextObject);

    // ---- 旧 BP 互換の最小 API ----
    UFUNCTION(BlueprintCallable, Category="UnitManager")
    void RegisterUnit(AActor* Unit, bool bIsAlly);

    UFUNCTION(BlueprintCallable, Category="UnitManager")
    void UnregisterUnit(AActor* Unit);

    UFUNCTION(BlueprintCallable, Category="UnitManager")
    void GetAllies(TArray<AActor*>& OutAllies) const;

    UFUNCTION(BlueprintCallable, Category="UnitManager")
    void GetEnemies(TArray<AActor*>& OutEnemies) const;

    // 破棄済み要素を除去したコピーを返す（BP/ログ用の安全API）
    UFUNCTION(BlueprintCallable, Category="UnitManager")
    TArray<AActor*> GetAlliesCopy() const;
    UFUNCTION(BlueprintCallable, Category="UnitManager")
    TArray<AActor*> GetEnemiesCopy() const;

protected:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

private:
    TArray<TWeakObjectPtr<AActor>> Allies;
    TArray<TWeakObjectPtr<AActor>> Enemies;
};
