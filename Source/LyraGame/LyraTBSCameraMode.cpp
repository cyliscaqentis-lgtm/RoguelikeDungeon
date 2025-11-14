#include "LyraTBSCameraMode.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

ULyraTBSCameraMode::ULyraTBSCameraMode()
{
    // ULyraCameraModeの公開プロパティ
    FieldOfView = 60.f;        // FOV（BPで変更可）
    ViewPitchMin = -89.9f;
    ViewPitchMax = 89.9f;

    // 初期値
    Distance = 1200.f;
    PitchDeg = 60.f;
    YawDeg = 45.f;
    PivotLocalOffset = FVector(0.f, 0.f, 80.f);

    FollowMode = ETBSCameraFollow::Continuous;
    PositionLagSpeed = 10.f;
    RotationLagSpeed = 15.f;

    bAllowPlayerYawInput = true;   // 既定：Yawはプレイヤー操作許可
    bAllowPlayerPitchInput = false;  // 既定：Pitchは固定値
    bAllowZoomInput = true;
    MinDistance = 300.f;
    MaxDistance = 3000.f;
    ZoomStep = 150.f;
}

void ULyraTBSCameraMode::AddZoom(float DeltaSteps)
{
    if (!bAllowZoomInput) return;
    Distance = FMath::Clamp(Distance - DeltaSteps * ZoomStep, MinDistance, MaxDistance);
    bResetInterpolation = true; // 次フレームはスナップさせて操作感を良くする
}

void ULyraTBSCameraMode::SetFixedAngles(float InYawDeg, float InPitchDeg)
{
    YawDeg = InYawDeg;
    PitchDeg = FMath::Clamp(InPitchDeg, ViewPitchMin, ViewPitchMax);
}

void ULyraTBSCameraMode::UpdateView(float DeltaTime)
{
    const AActor* Target = GetTargetActor();
    if (!Target) return;

    // --- 1) 角度決定（固定 or Controller）
    FRotator DesiredRot = FRotator(PitchDeg, YawDeg, 0.f);

    if (const APawn* Pawn = Cast<APawn>(Target))
    {
        if (const APlayerController* PC = Pawn->GetController<APlayerController>())
        {
            const FRotator Ctrl = PC->GetControlRotation();
            const float UseYaw = bAllowPlayerYawInput ? Ctrl.Yaw : YawDeg;
            const float UsePitch = bAllowPlayerPitchInput ? Ctrl.Pitch : PitchDeg;
            DesiredRot = FRotator(UsePitch, UseYaw, 0.f);
        }
    }

    // --- 2) ピボット（キャラの頭・カプセル高考慮）＋ローカルオフセット
    const FVector PivotWS = GetPivotLocation() + GetPivotRotation().RotateVector(PivotLocalOffset);

    // --- 3) 理想カメラ位置（視線ベクトルの反対方向にArm長ぶん）
    const FVector Back = DesiredRot.Vector() * -1.f;
    const FVector DesiredLoc = PivotWS + Back * Distance;

    // --- 4) ラグ or スナップ
    FVector  OutLoc = DesiredLoc;
    FRotator OutRot = DesiredRot;

    if (bResetInterpolation || FollowMode == ETBSCameraFollow::Snap || !bHasPrev)
    {
        bResetInterpolation = false;
        SmoothedLoc = DesiredLoc;
        SmoothedRot = DesiredRot;
        bHasPrev = true;
    }
    else
    {
        SmoothedLoc = FMath::VInterpTo(SmoothedLoc, DesiredLoc, DeltaTime, PositionLagSpeed);
        SmoothedRot = FMath::RInterpTo(SmoothedRot, DesiredRot, DeltaTime, RotationLagSpeed);
    }

    if (FollowMode == ETBSCameraFollow::Continuous)
    {
        OutLoc = SmoothedLoc;
        OutRot = SmoothedRot;
    }

    // --- 5) ビューを書き出し（LyraはViewに詰める）
    View.Location = OutLoc;
    View.Rotation = OutRot;
    View.ControlRotation = OutRot;   // ←CameraComponentがPCへ反映するので、ここでロック/許可を統制
    View.FieldOfView = FieldOfView;
}
