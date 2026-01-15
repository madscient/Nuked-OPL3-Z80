# Nuked-OPL3 for z88dk

Yamaha OPL3 (YMF262) エミュレータのz88dk移植版

## 概要

このプロジェクトは、nukeyktによる高精度なOPL3エミュレータ「Nuked-OPL3」を、z88dkコンパイラでビルドできるように移植したものです。

元のプロジェクト: https://github.com/nukeykt/Nuked-OPL3

## 対応プラットフォーム

z88dkがサポートする以下のプラットフォームで動作します:
- ZX Spectrum
- MSX
- Amstrad CPC
- CP/M systems
- その他Z80ベースのシステム

## 移植の主な変更点

### 1. データ型の調整
- `int16_t` → `signed short` (16-bit)
- `int32_t` → `signed long` (32-bit)
- `uint8_t` → `unsigned char`
- `uint16_t` → `unsigned short`
- `uint32_t` → `unsigned long`
- `uint64_t` → 構造体による実装 (z88dkは64bit整数未サポート)

### 2. メモリ最適化
- スタティックデータの削減
- ルックアップテーブルの最適化
- チップ構造体のサイズ削減

### 3. パフォーマンス最適化
- 浮動小数点演算の排除(既に整数演算のみ)
- z80に最適化されたアルゴリズム

## ビルド方法

### 前提条件
- z88dk (最新版推奨)
- GNU Make

### ZX Spectrumターゲット
```bash
cd nuked-opl3-z88dk
make spectrum
```

### MSXターゲット
```bash
make msx
```

### CP/Mターゲット
```bash
make cpm
```

### すべてのターゲット
```bash
make all
```

## 使用例

```c
#include "opl3.h"

opl3_chip chip;
signed short output[2];

// 初期化
OPL3_Reset(&chip, 49716); // サンプリングレート

// レジスタ書き込み
OPL3_WriteReg(&chip, 0x01, 0x20); // WSE=1

// サンプル生成
OPL3_GenerateStream(&chip, output, 1);
```

## ライセンス

元のNuked-OPL3と同じLGPL 2.1ライセンスを継承します。

## クレジット

- **オリジナル作者**: Alexey Khokholov (Nuke.YKT)
- **z88dk移植**: [あなたの名前]

## 謝辞

- Nuked-OPL3の作者であるnukeykt氏
- z88dkプロジェクト
- OPL3ハードウェアの解析に協力された方々

## 参考リンク

- [Nuked-OPL3 GitHub](https://github.com/nukeykt/Nuked-OPL3)
- [z88dk公式サイト](https://www.z88dk.org/)
- [OPL3リファレンス](https://www.fit.vutbr.cz/~arnost/opl/opl3.html)
