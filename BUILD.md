# ビルドコマンド

## UnrealBuildTool による直接ビルド（推奨）

```powershell
"C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" LyraEditor Win64 Development -Project="C:\UnrealProjects\RoguelikeDungeon\RoguelikeDungeon.uproject" -WaitMutex -NoHotReload -Verbose -Log="C:\UnrealProjects\RoguelikeDungeon\UBT.log"
```

### オプション説明

- `LyraEditor` - ターゲット名
- `Win64` - プラットフォーム
- `Development` - ビルド構成
- `-Project="..."` - プロジェクトファイルのパス
- `-WaitMutex` - 他のビルドプロセスが完了するまで待機
- `-NoHotReload` - ホットリロードを無効化（クリーンビルド）
- `-Verbose` - 詳細なログ出力
- `-Log="..."` - ログファイルの出力先

### ログファイル

ビルドログは以下の場所に出力されます：
- `C:\UnrealProjects\RoguelikeDungeon\UBT.log` （カスタムログ）
- `C:\Users\<ユーザー名>\AppData\Local\UnrealBuildTool\Log.txt` （デフォルトログ）

## シンプルビルド（ログなし）

```powershell
"C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" LyraEditor Win64 Development "C:\UnrealProjects\RoguelikeDungeon\RoguelikeDungeon.uproject" -WaitMutex
```

## Build.bat による標準ビルド

```cmd
"C:\Program Files\Epic Games\UE_5.6\Engine\Build\BatchFiles\Build.bat" LyraEditor Win64 Development -Project="C:\UnrealProjects\RoguelikeDungeon\RoguelikeDungeon.uproject" -WaitMutex -FromMsBuild
```
