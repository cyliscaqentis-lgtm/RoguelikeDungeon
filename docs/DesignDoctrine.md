## RoguelikeDungeon - Turn System Design Doctrine (2025-10-30)

この文書は、ターン制御の最終設計方針（是正版）の要点を開発者向けに簡潔にまとめたものです。

### 権威と状態遷移
- タグ・フェーズ・ゲート・Barrier 等のゲーム状態はサーバ（または Standalone を権威扱い）でのみ変更する。
- クライアントは OnRep で UI のみ更新。タグ操作はしない。

### 3-Tag System
- `Phase.Player.WaitInput` と `Gate.Input.Open` はサーバのみが Add/Remove。
- `OnRep_WaitingForPlayerInput` は UI 更新だけを行う。

### イベント/ペイロード規約
- `EventMagnitude` は方向エンコード専用（整数パック: 1000 + (X+1)*100 + (Y+1)）。
- 将来的に TurnId は `InstigatorTags` or `ContextHandle` に格納して照合（現状は照合しない）。

### Barrier
- `StartMoveBatch` は実際に敵移動を流す直前（`ExecuteMovePhase`）の1箇所だけで開始。
- 完了通知は実行サブシステムが行い、TurnManager は手で Notify しない。

### 敵収集
- `Faction.Enemy` GameplayTag のみで収集する（TeamID/ActorTag に依存しない）。
- 毎回 `CachedEnemies` をクリアして再構築する。

### パイプライン責務
- TurnManager: フェーズの指揮・ゲート管理・パイプライン接続。
- EnemyAISubsystem: Observation/Intent の唯一の生産者。
- ActionExecutorSubsystem: プレイヤー/敵の移動実行と完了通知の窓口。
- AttackPhaseExecutorSubsystem: 攻撃解決と逐次実行（完了通知）。
- TurnCorePhaseManager: 攻撃/移動の解決集約。

### 旧API
- `RunTurn()` は Deprecated（呼ばれても何もしない設計）。
- 手動 `OnRep_*` 呼び出しは禁止。

### 二重鍵（Advance条件）
1) `Barrier.IsQuiescent(TurnId)` が true
2) `ASC.GetTagCount(State.Action.InProgress) == 0`
両方満たした時のみ Advance。

### 運用ガイド
- ログレベル: 重要は Warning、正常フローは Log、大量出力は Verbose。
- ターンごとに要点サマリ（敵数・観測・インテント・バッチ数）を1行で出す。




