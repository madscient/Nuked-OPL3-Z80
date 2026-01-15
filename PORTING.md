# z88dk移植ガイド

## z88dkの制約と対応策

### 1. 64bit整数型のサポート不足

z88dkは`uint64_t`をサポートしていません。オリジナルのNuked-OPL3では64bit演算が使用されています。

**対応策:**
- 64bit値を2つの32bit値の構造体で表現
- 64bit演算を32bit演算の組み合わせで実装
- または、該当部分をアセンブラで最適化実装

```c
typedef struct {
    unsigned long low;   // 下位32bit
    unsigned long high;  // 上位32bit
} uint64_z88dk;
```

### 2. メモリ制約

多くのZ80システムは64KB以下のRAMしか持ちません。

**対応策:**
- opl3_chip構造体のサイズを最小化
- 大きなルックアップテーブルはROMに配置 (`const`修飾)
- バンク切り替えによるメモリ拡張の活用(可能な場合)

### 3. パフォーマンス

Z80は3.5MHzなど、低速なCPUです。

**対応策:**
- 重要な関数はインラインアセンブラで実装
- ルックアップテーブルの活用
- 不要な計算の削減
- 固定小数点演算の使用

## 移植手順

### ステップ1: ヘッダーファイルの調整

`opl3.h`の修正点:
```c
// 標準整数型の置き換え
#ifndef __Z88DK__
#include <stdint.h>
#else
// z88dk用の型定義
typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef signed short int16_t;
typedef unsigned long uint32_t;
typedef signed long int32_t;

// 64bit型の代替実装
typedef struct {
    unsigned long low;
    unsigned long high;
} uint64_t;
#endif
```

### ステップ2: ソースコードの調整

1. **includeの追加**
   ```c
   #ifdef __Z88DK__
   #include <arch/zx.h>  // ターゲット固有
   #endif
   ```

2. **メモリ配置の最適化**
   ```c
   // ルックアップテーブルをROMに配置
   const uint16_t eg_table[4096] = { /* ... */ };
   ```

3. **64bit演算の置き換え**
   - ビットシフト
   - 加算・減算
   - 乗算(必要な場合)

### ステップ3: ビルドシステムの作成

`Makefile`の例:
```makefile
CC = zcc
CFLAGS = +zx -vn -SO3 --max-allocs-per-node200000

# ソースファイル
SOURCES = src/opl3.c examples/simple_test.c

# ターゲット
spectrum: $(SOURCES)
	$(CC) $(CFLAGS) -o build/opl3test.bin $(SOURCES) -create-app

msx:
	$(CC) +msx -vn -SO3 -o build/opl3test.com $(SOURCES)

cpm:
	$(CC) +cpm -vn -SO3 -o build/opl3test.com $(SOURCES)
```

### ステップ4: テストプログラムの作成

シンプルなテストプログラム:
```c
#include <stdio.h>
#include "opl3.h"

int main(void) {
    opl3_chip chip;
    signed short buf[2];
    
    printf("Nuked-OPL3 Test\n");
    
    // 初期化
    OPL3_Reset(&chip, 49716);
    
    // テストトーンを設定
    OPL3_WriteReg(&chip, 0x01, 0x20);  // WSE=1
    OPL3_WriteReg(&chip, 0x20, 0x01);  // Mult=1
    OPL3_WriteReg(&chip, 0x40, 0x10);  // KSL/TL
    OPL3_WriteReg(&chip, 0x60, 0xF0);  // AR/DR
    OPL3_WriteReg(&chip, 0x80, 0x77);  // SL/RR
    OPL3_WriteReg(&chip, 0xA0, 0x98);  // FNUM low
    OPL3_WriteReg(&chip, 0xB0, 0x31);  // Key ON, BLOCK, FNUM high
    
    // サンプル生成
    for (int i = 0; i < 100; i++) {
        OPL3_GenerateStream(&chip, buf, 1);
        printf("%d,%d\n", buf[0], buf[1]);
    }
    
    return 0;
}
```

## メモリマップ例 (ZX Spectrum)

```
0x0000-0x3FFF: ROM (16KB)
0x4000-0x5AFF: Screen (6912 bytes)
0x5B00-0xFFFF: RAM (42KB for program)
```

プログラム配置:
- コード: 0x5B00~
- データ: スタック前の領域
- スタック: 0xFF00から下方向に成長

## パフォーマンス測定

サンプル生成速度の目標:
- 49716 Hz sampling rate
- 1サンプル = 約70 T-states (3.5MHz CPUで)
- 目標: リアルタイム再生可能

最適化が必要な関数:
1. `OPL3_SlotCalcFB()` - フィードバック計算
2. `OPL3_EnvelopeCalc()` - エンベロープ計算
3. `OPL3_PhaseGenerate()` - 位相生成

## トラブルシューティング

### コンパイルエラー

**"undefined reference to uint64_t"**
→ 64bit型の代替実装を確認

**"out of memory"**
→ ルックアップテーブルに`const`を付けてROMに配置

### 実行時エラー

**音が出ない**
→ サンプリングレートとタイミングを確認

**音が歪む**
→ 整数オーバーフローをチェック

## 追加リソース

- [z88dk Wiki](https://github.com/z88dk/z88dk/wiki)
- [Z80 Instruction Set](http://www.z80.info/z80code.htm)
- [OPL3 Programming Guide](https://www.fit.vutbr.cz/~arnost/opl/opl3.html)
