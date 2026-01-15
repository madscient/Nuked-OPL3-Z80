# Nuked-OPL3 for z88dk - クイックスタートガイド

## はじめに

このプロジェクトは、高精度なYamaha OPL3エミュレータ「Nuked-OPL3」をZ80ベースのレトロコンピュータで動作させるための移植プロジェクトです。

## 現在の状態

**開発初期段階** - 基本的なプロジェクト構造とスケルトンコードが完成しています。

**完了済み:**
- ✓ プロジェクト構造
- ✓ ヘッダーファイル
- ✓ 64bit演算ヘルパー
- ✓ ビルドシステム
- ✓ サンプルプログラム

**未完了(重要):**
- ⚠ コア実装(opl3.c)の大部分
- ⚠ ルックアップテーブルのデータ
- ⚠ 実際の音声生成機能

## 次のステップ

移植を完成させるには、以下の手順で作業を進めてください:

### ステップ1: オリジナルソースの入手

```bash
# オリジナルのNuked-OPL3をクローン
git clone https://github.com/nukeykt/Nuked-OPL3.git original-nuked-opl3

# ファイルを確認
cd original-nuked-opl3
ls -la
```

### ステップ2: ルックアップテーブルの移植

`original-nuked-opl3/opl3.c`から以下のテーブルをコピー:
- `logsinrom[]`
- `exprom[]`
- その他の定数配列

これらを`src/opl3.c`の該当箇所に貼り付けます。

### ステップ3: 関数の移植

1. **小さい関数から始める:**
   - `OPL3_EnvelopeUpdateKSL()`
   - `OPL3_EnvelopeCalcRate()`
   - `OPL3_SlotCalcPhase()`

2. **データ型を調整:**
   ```c
   // 元のコード
   uint64_t value = ...;
   
   // z88dk版
   uint64_t value;
   uint64_init(&value, low, high);
   ```

3. **64bit演算を置き換え:**
   ```c
   // 元のコード
   value = value + increment;
   
   // z88dk版
   uint64_add32(&value, &value, increment);
   ```

### ステップ4: コンパイルとテスト

```bash
# プロジェクトディレクトリに移動
cd nuked-opl3-z88dk

# ZX Spectrum版をビルド
make spectrum

# エラーがあれば修正
# 成功すれば build/opl3_spectrum.tap が生成される
```

### ステップ5: エミュレータでテスト

```bash
# Fuseエミュレータでテスト(Linuxの場合)
fuse-emulator build/opl3_spectrum.tap

# またはRetroArchなど他のエミュレータを使用
```

## ファイル構造

```
nuked-opl3-z88dk/
├── README.md              # プロジェクト概要
├── PORTING.md            # 移植の詳細ガイド
├── TODO.md               # 作業リスト
├── QUICKSTART.md         # このファイル
├── Makefile              # ビルドシステム
├── include/              # ヘッダーファイル
│   ├── opl3.h           # メインヘッダー
│   └── opl3_int64.h     # 64bit演算ヘルパー
├── src/                  # ソースコード
│   └── opl3.c           # メイン実装(要移植)
├── examples/             # サンプルプログラム
│   ├── simple_test.c    # 基本テスト
│   └── scale_demo.c     # 音階デモ
└── build/                # ビルド成果物(自動生成)
```

## よくある問題と解決法

### Q1: "uint64_t undefined" エラー

**A:** z88dk用のヘッダーが正しくインクルードされていません。
```c
#include "opl3.h"  // これにuint64_tの定義が含まれています
```

### Q2: "out of memory" エラー

**A:** ルックアップテーブルに`const`を付けてROMに配置します:
```c
const uint16_t logsinrom[256] = { ... };
```

### Q3: コンパイルは成功するが音が出ない

**A:** 以下を確認:
1. `OPL3_Reset()`が正しく実装されているか
2. `OPL3_WriteReg()`がレジスタを正しく処理しているか
3. `OPL3_Generate()`が実際にサンプルを生成しているか

### Q4: 実行速度が遅すぎる

**A:** 最適化オプションを調整:
```bash
# Makefileで最適化レベルを上げる
COMMON_FLAGS = -vn -SO3 --opt-code-speed
```

## 推奨する開発環境

### Linux/Unix
```bash
# z88dkのインストール(Ubuntu/Debian)
sudo apt-get install z88dk

# または最新版をビルド
git clone https://github.com/z88dk/z88dk.git
cd z88dk
./build.sh
```

### Windows
1. z88dkのWindows版をダウンロード
2. 環境変数PATHに追加
3. MinGW/MSYSまたはWSLを使用

### macOS
```bash
# Homebrewでインストール
brew install z88dk
```

## 参考リソース

### ドキュメント
- [z88dk Wiki](https://github.com/z88dk/z88dk/wiki)
- [OPL3リファレンス](https://www.fit.vutbr.cz/~arnost/opl/opl3.html)
- [Z80命令セット](http://www.z80.info/z80code.htm)

### コミュニティ
- [z88dk Forums](https://www.z88dk.org/forum/)
- [World of Spectrum](https://worldofspectrum.org/)
- [VOGONS Forum](https://www.vogons.org/)

### エミュレータ
- **ZX Spectrum:** Fuse, ZEsarUX, SpecEmu
- **MSX:** openMSX, blueMSX
- **CP/M:** z80pack, SimH
- **Amstrad CPC:** WinAPE, Arnold

## 貢献とフィードバック

このプロジェクトはオープンソースです。改善提案やバグレポートを歓迎します!

### 貢献方法
1. このプロジェクトをフォーク
2. 機能ブランチを作成(`git checkout -b feature/amazing-feature`)
3. 変更をコミット(`git commit -m 'Add amazing feature'`)
4. ブランチにプッシュ(`git push origin feature/amazing-feature`)
5. プルリクエストを作成

## ライセンス

このプロジェクトは元のNuked-OPL3と同じLGPL 2.1ライセンスです。

詳細は`LICENSE`ファイルを参照してください。

## クレジット

- **Nuked-OPL3作者:** Alexey Khokholov (Nuke.YKT)
- **z88dk開発チーム:** 素晴らしいツールチェーンの提供
- **OPL3研究者:** ハードウェア解析と文書化

---

**注意:** このクイックスタートガイドは、プロジェクトの初期段階での使用を想定しています。実装が進むにつれて、内容は更新される可能性があります。
