// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraDamageExecution.h"
#include "AbilitySystem/Attributes/LyraHealthSet.h"
#include "AbilitySystem/Attributes/LyraCombatSet.h"
#include "AbilitySystem/LyraGameplayEffectContext.h"
#include "AbilitySystem/LyraAbilitySourceInterface.h"
#include "Engine/World.h"
#include "LyraLogChannels.h"
#include "Teams/LyraTeamSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraDamageExecution)

struct FDamageStatics
{
	FGameplayEffectAttributeCaptureDefinition BaseDamageDef;

	FDamageStatics()
	{
		BaseDamageDef = FGameplayEffectAttributeCaptureDefinition(
			ULyraCombatSet::GetBaseDamageAttribute(),
			EGameplayEffectAttributeCaptureSource::Source,
			true);
	}
};

static FDamageStatics& DamageStatics()
{
	static FDamageStatics Statics;
	return Statics;
}

ULyraDamageExecution::ULyraDamageExecution()
{
	RelevantAttributesToCapture.Add(DamageStatics().BaseDamageDef);
}

void ULyraDamageExecution::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
#if WITH_SERVER_CODE
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FLyraGameplayEffectContext* TypedContext =
		FLyraGameplayEffectContext::ExtractEffectContext(Spec.GetContext());
	check(TypedContext);

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float BaseDamage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().BaseDamageDef,
		EvaluateParameters,
		BaseDamage);

	const AActor* EffectCauser = TypedContext->GetEffectCauser();
	const FHitResult* HitActorResult = TypedContext->GetHitResult();

	AActor* HitActor = nullptr;
	FVector ImpactLocation = FVector::ZeroVector;
	FVector ImpactNormal = FVector::ZeroVector;
	FVector StartTrace = FVector::ZeroVector;
	FVector EndTrace = FVector::ZeroVector;

	if (HitActorResult)
	{
		const FHitResult& CurHitResult = *HitActorResult;
		HitActor = CurHitResult.HitObjectHandle.FetchActor();
		if (HitActor)
		{
			ImpactLocation = CurHitResult.ImpactPoint;
			ImpactNormal = CurHitResult.ImpactNormal;
			StartTrace = CurHitResult.TraceStart;
			EndTrace = CurHitResult.TraceEnd;
		}
	}

	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	if (!HitActor)
	{
		HitActor = TargetAbilitySystemComponent
			? TargetAbilitySystemComponent->GetAvatarActor_Direct()
			: nullptr;
		if (HitActor)
		{
			ImpactLocation = HitActor->GetActorLocation();
		}
	}

	float DamageInteractionAllowedMultiplier = 0.0f;
	if (HitActor)
	{
		ULyraTeamSubsystem* TeamSubsystem =
			HitActor->GetWorld()->GetSubsystem<ULyraTeamSubsystem>();
		if (ensure(TeamSubsystem))
		{
			DamageInteractionAllowedMultiplier =
				TeamSubsystem->CanCauseDamage(EffectCauser, HitActor)
				? 1.0f
				: 0.0f;
		}
	}

	double Distance = WORLD_MAX;

	if (TypedContext->HasOrigin())
	{
		Distance = FVector::Dist(TypedContext->GetOrigin(), ImpactLocation);
	}
	else if (EffectCauser)
	{
		Distance = FVector::Dist(EffectCauser->GetActorLocation(), ImpactLocation);
	}
	else
	{
		UE_LOG(LogLyraAbilitySystem,
			Error,
			TEXT("Damage Calculation cannot deduce a source location for damage coming from %s; Falling back to WORLD_MAX dist!"),
			*GetPathNameSafe(Spec.Def));
	}

	float PhysicalMaterialAttenuation = 1.0f;
	float DistanceAttenuation = 1.0f;
	if (const ILyraAbilitySourceInterface* AbilitySource =
		TypedContext->GetAbilitySource())
	{
		if (const UPhysicalMaterial* PhysMat =
			TypedContext->GetPhysicalMaterial())
		{
			PhysicalMaterialAttenuation = AbilitySource->GetPhysicalMaterialAttenuation(
				PhysMat,
				SourceTags,
				TargetTags);
		}

		DistanceAttenuation = AbilitySource->GetDistanceAttenuation(
			Distance,
			SourceTags,
			TargetTags);
	}
	DistanceAttenuation = FMath::Max(DistanceAttenuation, 0.0f);

	// ======== ‚±‚±‚©‚çƒƒO’Ç‰Á =========
	const FString SrcName = EffectCauser ? EffectCauser->GetName() : TEXT("None");
	const FString TgtName = HitActor ? HitActor->GetName() : TEXT("None");

	UE_LOG(LogTemp, Warning, TEXT("[DMG] Src=%s  Tgt=%s"), *SrcName, *TgtName);
	UE_LOG(LogTemp, Warning, TEXT("[DMG] BaseDamage=%.2f  DistAtten=%.2f  PhysMatAtten=%.2f  Mult=%.2f"),
		BaseDamage, DistanceAttenuation, PhysicalMaterialAttenuation, DamageInteractionAllowedMultiplier);
	// ======== ‚±‚±‚Ü‚ÅƒƒO’Ç‰Á =========

	const float DamageDone =
		FMath::Max(BaseDamage * DistanceAttenuation *
			PhysicalMaterialAttenuation *
			DamageInteractionAllowedMultiplier,
			0.0f);

	UE_LOG(LogTemp, Warning, TEXT("[DMG] => DamageDone=%.2f"), DamageDone);

	if (DamageDone > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
			ULyraHealthSet::GetDamageAttribute(),
			EGameplayModOp::Additive,
			DamageDone));
		UE_LOG(LogTemp, Warning, TEXT("[DMG] Applied modifier to target (HealthSet.Damage += %.2f)"), DamageDone);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[DMG] Skipped: DamageDone <= 0 (no modifier added)"));
	}
#endif // #if WITH_SERVER_CODE
}
