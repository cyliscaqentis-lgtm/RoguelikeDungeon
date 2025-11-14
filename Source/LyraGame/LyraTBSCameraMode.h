#pragma once

#include "CoreMinimal.h"
#include "Camera/LyraCameraMode.h"
#include "LyraTBSCameraMode.generated.h"

// 追従方式（連続ラグ or スナップ）
UENUM(BlueprintType)
enum class ETBSCameraFollow : uint8
{
    Continuous UMETA(DisplayName = "Continuous (Lag)"),
    Snap       UMETA(DisplayName = "Snap (No Lag)")
};

UCLASS(Blueprintable) // ← BPから扱えるように
class LYRAGAME_API ULyraTBSCameraMode : public ULyraCameraMode
{
    GENERATED_BODY()

public:
    ULyraTBSCameraMode();

    // ---- Arm的コントロール ----
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TBS|View", meta = (ClampMin = "100.0"))
    float Distance;                        // Arm長

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TBS|View", meta = (ClampMin = "-89.9", ClampMax = "89.9"))
    float PitchDeg;                        // 俯角（固定値 or Controller）

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TBS|View")
    float YawDeg;                          // 水平角（固定値 or Controller）

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TBS|View")
    FVector PivotLocalOffset;              // ターゲット原点からのローカルオフセット（肩上げ等）

    // ---- 追従挙動 ----
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TBS|Lag")
    ETBSCameraFollow FollowMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TBS|Lag", meta = (EditCondition = "FollowMode==ETBSCameraFollow::Continuous"))
    float PositionLagSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TBS|Lag", meta = (EditCondition = "FollowMode==ETBSCameraFollow::Continuous"))
    float RotationLagSpeed;

    // ---- 視点変更の許可 ----
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TBS|Input")
    bool bAllowPlayerYawInput;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TBS|Input")
    bool bAllowPlayerPitchInput;

    // ---- ズーム（必要なら）----
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TBS|Zoom")
    bool bAllowZoomInput;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TBS|Zoom", meta = (ClampMin = "100.0"))
    float MinDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TBS|Zoom", meta = (ClampMin = "100.0"))
    float MaxDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TBS|Zoom")
    float ZoomStep;

    UFUNCTION(BlueprintCallable, Category = "TBS|Zoom")
    void AddZoom(float DeltaSteps);        // +1/-1などで呼ぶ

    UFUNCTION(BlueprintCallable, Category = "TBS|View")
    void SetFixedAngles(float InYawDeg, float InPitchDeg);

protected:
    virtual void UpdateView(float DeltaTime) override; // ←正しいシグネチャ

private:
    bool bHasPrev = false;
    FVector  SmoothedLoc;
    FRotator SmoothedRot;
};
