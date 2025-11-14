// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LyraGame : ModuleRules
{
    public LyraGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // ======== Include Path 險ｭ螳・========
        PublicIncludePaths.AddRange(
            new string[] {
                "LyraGame",
                "LyraGame/Rogue"   // 竊・譌｢蟄假ｼ啌ogue繝輔か繝ｫ繝莉･荳九ｒ隱崎ｭ・
            }
        );

        PrivateIncludePaths.AddRange(
            new string[] {
                "LyraGame",
                "LyraGame/Rogue"   // 竊・譌｢蟄假ｼ啀rivate繝倥ャ繝蜿ら・逕ｨ
            }
        );

        // ======== Public萓晏ｭ倥Δ繧ｸ繝･繝ｼ繝ｫ ========
        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreOnline",
                "CoreUObject",
                "ApplicationCore",
                "Engine",
                "PhysicsCore",
                "GameplayTags",
                "GameplayTasks",
                "GameplayAbilities",
                "AIModule",
                "ModularGameplay",
                "ModularGameplayActors",
                "DataRegistry",
                "ReplicationGraph",
                "GameFeatures",
                "SignificanceManager",
                "Hotfix",
                "CommonLoadingScreen",
                "Niagara",
                "AsyncMixin",
                "ControlFlows",
                "PropertyPath",
                "StateTreeModule",
                "GameplayStateTreeModule"
            }
        );

        // ======== Private萓晏ｭ倥Δ繧ｸ繝･繝ｼ繝ｫ ========
        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "InputCore",
                "Slate",
                "SlateCore",
                "RenderCore",
                "DeveloperSettings",
                "EnhancedInput",
                "NetCore",
                "RHI",
                "Projects",
                "Gauntlet",
                "UMG",
                "CommonUI",
                "CommonInput",
                "GameSettings",
                "CommonGame",
                "CommonUser",
                "GameSubtitles",
                "GameplayMessageRuntime",
                "AudioMixer",
                "NetworkReplayStreaming",
                "UIExtension",
                "ClientPilot",
                "AudioModulation",
                "EngineSettings",
                "DTLSHandlerComponent",
                "Json",
                "ExternalRpcRegistry",
                "HTTPServer"
            }
        );

        // ======== v2.2霑ｽ蜉・壹ョ繝舌ャ繧ｰ繝薙ず繝･繧｢繝ｩ繧､繧ｼ繝ｼ繧ｷ繝ｧ繝ｳ逕ｨ ========
        // Enable draw-debug helpers while developing.
        if (Target.Configuration != UnrealTargetConfiguration.Shipping)
        {
            PublicDefinitions.Add("WITH_DRAW_DEBUG=1");
        }

        PublicDefinitions.Add("SHIPPING_DRAW_DEBUG_ERROR=1");

        // ======== v2.2霑ｽ蜉・售ubsystem讖溯・遒ｺ隱・========
        // StableActorRegistry, ConflictResolverSubsystem縺ｧ菴ｿ逕ｨ
        // 譌｢蟄倥・Engine繝｢繧ｸ繝･繝ｼ繝ｫ縺ｧ謠蝉ｾ帙＆繧後ｋ縺溘ａ霑ｽ蜉荳崎ｦ・
        // ・育｢ｺ隱阪・縺溘ａ繧ｳ繝｡繝ｳ繝郁ｨ倩ｼ会ｼ・
        // - UWorldSubsystem: Engine繝｢繧ｸ繝･繝ｼ繝ｫ
        // - FGuid逕滓・: CoreUObject繝｢繧ｸ繝･繝ｼ繝ｫ

        // ======== 譌｢蟄假ｼ哦ridPathfindingLibrary逕ｨ縺ｮ險ｭ螳・========
        if (Target.Configuration == UnrealTargetConfiguration.Shipping)
        {
            PublicDefinitions.Add("WITH_RPC_REGISTRY=0");
            PublicDefinitions.Add("WITH_HTTPSERVER_LISTENERS=0");
        }
        else
        {
            PublicDefinitions.Add("WITH_RPC_REGISTRY=1");
            PublicDefinitions.Add("WITH_HTTPSERVER_LISTENERS=1");
        }

        // ======== v2.2霑ｽ蜉・壽ｱｺ螳壽ｧ讀懆ｨｼ逕ｨ縺ｮ繝励Ο繝輔ぃ繧､繝ｪ繝ｳ繧ｰ ========
        // SCOPE_CYCLE_COUNTER繝槭け繝ｭ菴ｿ逕ｨ縺ｮ縺溘ａ縲・幕逋ｺ繝薙Ν繝峨〒邨ｱ險域怏蜉ｹ蛹・
        // STATS縺ｯEngine縺ｧ譌｢縺ｫ螳夂ｾｩ貂医∩縺ｪ縺ｮ縺ｧ驥崎､・ｒ驕ｿ縺代ｋ
        if (Target.Configuration == UnrealTargetConfiguration.Development ||
            Target.Configuration == UnrealTargetConfiguration.DebugGame)
        {
            // PublicDefinitions.Add("STATS=1");  // 竊・蜑企勁・哘ngine縺ｧ譌｢縺ｫ螳夂ｾｩ貂医∩
        }

        // ======== ネイティブタグを確実にビルド対象に含める ========
        // RogueGameplayTags.cpp で GameplayTags を一元登録
        // GameplayTagsモジュールは既にPublicDependencyModuleNamesに含まれているため追加不要

        SetupGameplayDebuggerSupport(Target);
        SetupIrisSupport(Target);
    }
}



