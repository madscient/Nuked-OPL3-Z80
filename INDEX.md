# Nuked-OPL3 for z88dk - プロジェクトインデックス

## プロジェクト概要

このプロジェクトは、Alexey Khokholov (Nuke.YKT)氏による高精度なYamaha OPL3 (YMF262)エミュレータ「Nuked-OPL3」を、z88dkツールチェーンを使ってZ80ベースのレトロコンピュータ向けに移植するものです。

**オリジナルプロジェクト:** https://github.com/nukeykt/Nuked-OPL3

## ドキュメント一覧

### 📚 基本ドキュメント

1. **[README.md](README.md)** - プロジェクトの概要
   - プロジェクトの目的
   - 対応プラットフォーム
   - 基本的な使用方法
   - ライセンス情報

2. **[QUICKSTART.md](QUICKSTART.md)** - クイックスタートガイド ⭐
   - すぐに始めるための手順
   - 現在の開発状況
   - 次のステップ
   - よくある問題と解決法

3. **[PORTING.md](PORTING.md)** - 移植の詳細ガイド
   - z88dkの制約と対応策
   - 移植手順の詳細
   - メモリマップ
   - パフォーマンス最適化

4. **[TODO.md](TODO.md)** - 作業リストと進捗管理
   - 移植作業のフェーズ
   - 優先順位
   - 既知の課題
   - 進捗状況

## ソースコード構造

### 📁 ディレクトリ構成

```
nuked-opl3-z88dk/
│
├── 📄 README.md                  # プロジェクト概要
├── 📄 QUICKSTART.md             # クイックスタート ⭐
├── 📄 PORTING.md                # 移植ガイド
├── 📄 TODO.md                   # 作業リスト
├── 📄 INDEX.md                  # このファイル
├── 📄 Makefile                  # ビルドシステム
│
├── 📁 include/                  # ヘッダーファイル
│   ├── opl3.h                  # メインヘッダー
│   └── opl3_int64.h            # 64bit演算ヘルパー
│
├── 📁 src/                      # ソースコード
│   └── opl3.c                  # メイン実装(要移植)
│
├── 📁 examples/                 # サンプルプログラム
│   ├── simple_test.c           # 基本動作テスト
│   └── scale_demo.c            # 音階演奏デモ
│
└── 📁 build/                    # ビルド成果物(自動生成)
```

### 🔧 コアファイル

#### ヘッダーファイル

1. **[include/opl3.h](include/opl3.h)**
   - OPL3チップの構造体定義
   - 公開API関数のプロトタイプ
   - z88dk互換の型定義

2. **[include/opl3_int64.h](include/opl3_int64.h)**
   - 64bit整数演算のヘルパー関数
   - z88dk用の実装
   - 通常のコンパイラ用のマクロ

#### 実装ファイル

3. **[src/opl3.c](src/opl3.c)**
   - OPL3エミュレータのメイン実装
   - **現在:** スケルトンコード
   - **TODO:** オリジナルから機能を移植

### 📝 サンプルプログラム

4. **[examples/simple_test.c](examples/simple_test.c)**
   - 基本的な動作確認プログラム
   - チップの初期化
   - テストトーンの生成
   - サンプル出力

5. **[examples/scale_demo.c](examples/scale_demo.c)**
   - 音階演奏のデモ
   - ド・レ・ミ・ファ・ソ・ラ・シ・ド
   - 楽器設定の例

## ビルドシステム

### 📦 Makefileターゲット

```bash
# すべてのプラットフォーム向けにビルド
make all

# 個別のプラットフォーム
make spectrum      # ZX Spectrum (.tap)
make spectrum128   # ZX Spectrum 128K
make msx          # MSX (.com)
make cpm          # CP/M (.com)
make amstrad      # Amstrad CPC (.cdt)

# ユーティリティ
make clean        # ビルド成果物を削除
make asm          # アセンブリ出力を生成
make library      # ライブラリとしてビルド
make size         # バイナリサイズを確認
make help         # ヘルプを表示
```

## 開発ワークフロー

### 🚀 推奨される開発手順

1. **準備**
   - [QUICKSTART.md](QUICKSTART.md)を読む
   - z88dkをインストール
   - オリジナルのNuked-OPL3を入手

2. **実装**
   - [PORTING.md](PORTING.md)の手順に従う
   - [TODO.md](TODO.md)のタスクを進める
   - 小さい関数から移植開始

3. **テスト**
   - ビルドして動作確認
   - エミュレータでテスト
   - パフォーマンス測定

4. **最適化**
   - ホットスポットを特定
   - メモリ使用量を削減
   - 必要に応じてアセンブラ最適化

## 現在の開発状況

### ✅ 完了項目

- ✓ プロジェクト構造の作成
- ✓ ビルドシステムの構築
- ✓ ヘッダーファイルの作成
- ✓ 64bit演算ヘルパーの実装
- ✓ サンプルプログラムの作成
- ✓ ドキュメントの作成

### ⚠️ 進行中/未完了

- ⚠️ コア機能の実装(最重要)
- ⚠️ ルックアップテーブルのデータ
- ⚠️ 各種最適化
- ⚠️ プラットフォーム別テスト
- ⚠️ パフォーマンステスト

**全体進捗: 約15%**

詳細は[TODO.md](TODO.md)を参照してください。

## 技術的な課題

### 主な制約

1. **64bit整数のサポート不足**
   - 解決策: 構造体による実装([opl3_int64.h](include/opl3_int64.h))

2. **限られたメモリ**
   - ZX Spectrum 48K: 約42KB RAM
   - 解決策: ルックアップテーブルのROM配置

3. **低速なCPU**
   - Z80 @ 3.5MHz
   - 解決策: 最適化とアセンブラ実装

## 対応プラットフォーム

### 🎮 テスト済み(予定)

- [ ] ZX Spectrum 48K/128K
- [ ] MSX
- [ ] CP/M (Generic)
- [ ] Amstrad CPC

### 🎯 将来的に対応予定

- Sega Master System
- TRS-80
- Sord M5
- その他Z80ベースシステム

## リソースとリンク

### 📖 ドキュメント

- [z88dk公式サイト](https://www.z88dk.org/)
- [z88dk Wiki](https://github.com/z88dk/z88dk/wiki)
- [OPL3リファレンス](https://www.fit.vutbr.cz/~arnost/opl/opl3.html)
- [Z80命令セット](http://www.z80.info/z80code.htm)

### 🛠️ ツール

- [Fuse Emulator](http://fuse-emulator.sourceforge.net/) - ZX Spectrum
- [openMSX](https://openmsx.org/) - MSX
- [z80pack](http://www.autometer.de/unix4fun/z80pack/) - CP/M

### 💬 コミュニティ

- [z88dk Forums](https://www.z88dk.org/forum/)
- [VOGONS](https://www.vogons.org/) - Vintage Computer Forum
- [World of Spectrum](https://worldofspectrum.org/)

## ライセンス

このプロジェクトは**LGPL 2.1**ライセンスの下で公開されています。

元のNuked-OPL3プロジェクトのライセンスを継承しています。

## クレジット

### オリジナル作者
- **Alexey Khokholov (Nuke.YKT)** - Nuked-OPL3の作者

### z88dk移植
- **[あなたの名前]** - z88dk移植版の開発

### 謝辞
- z88dkプロジェクトチーム
- OPL3ハードウェア解析に貢献された方々
- レトロコンピューティングコミュニティ

## サポートと貢献

### 🐛 バグレポート

問題を発見した場合は、GitHubのIssueで報告してください。

### 🤝 貢献

プルリクエストを歓迎します!大きな変更の場合は、まずIssueで相談してください。

### 📧 連絡先

- GitHub: [プロジェクトURL]
- Email: [連絡先メールアドレス]

---

**最終更新:** 2025年1月
**バージョン:** 0.1.0-alpha (開発初期段階)
