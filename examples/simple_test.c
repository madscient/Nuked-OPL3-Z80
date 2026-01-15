/*
 * Simple test program for Nuked-OPL3 on z88dk
 * 
 * このプログラムはOPL3エミュレータの基本的な動作をテストします
 */

#include <stdio.h>
#include <stdlib.h>
#include "opl3.h"

/* グローバル変数 */
static opl3_chip chip;

/* テストトーンのレジスタ設定 */
void setup_test_tone(void) {
    printf("Setting up test tone...\n");
    
    /* OPL3モードを有効化 */
    OPL3_WriteReg(&chip, 0x105, 0x01);  /* OPL3有効 */
    OPL3_WriteReg(&chip, 0x104, 0x00);  /* 4op無効 */
    
    /* チャンネル0のオペレータ1(キャリア)を設定 */
    OPL3_WriteReg(&chip, 0x20, 0x01);   /* AM=0, VIB=0, EGT=0, KSR=0, MULT=1 */
    OPL3_WriteReg(&chip, 0x40, 0x10);   /* KSL=0, TL=16 */
    OPL3_WriteReg(&chip, 0x60, 0xF0);   /* AR=15, DR=0 */
    OPL3_WriteReg(&chip, 0x80, 0x77);   /* SL=7, RR=7 */
    OPL3_WriteReg(&chip, 0xE0, 0x00);   /* WS=0(sine) */
    
    /* チャンネル0のオペレータ0(モジュレータ)を設定 */
    OPL3_WriteReg(&chip, 0x23, 0x01);   /* AM=0, VIB=0, EGT=0, KSR=0, MULT=1 */
    OPL3_WriteReg(&chip, 0x43, 0x00);   /* KSL=0, TL=0 */
    OPL3_WriteReg(&chip, 0x63, 0xF0);   /* AR=15, DR=0 */
    OPL3_WriteReg(&chip, 0x83, 0x77);   /* SL=7, RR=7 */
    OPL3_WriteReg(&chip, 0xE3, 0x00);   /* WS=0(sine) */
    
    /* チャンネル0の設定 */
    OPL3_WriteReg(&chip, 0xC0, 0x01);   /* FB=0, ALG=1(FM) */
    
    /* 周波数を設定(A4 = 440Hz) */
    OPL3_WriteReg(&chip, 0xA0, 0x98);   /* F-Number low */
    OPL3_WriteReg(&chip, 0xB0, 0x31);   /* Key ON, Block=3, F-Number high */
    
    printf("Test tone configured.\n");
}

/* テストトーンを停止 */
void stop_test_tone(void) {
    printf("Stopping test tone...\n");
    OPL3_WriteReg(&chip, 0xB0, 0x11);   /* Key OFF */
}

/* メイン関数 */
int main(void) {
    int16_t samples[2];
    int i;
    
    printf("Nuked-OPL3 Test Program\n");
    printf("=======================\n\n");
    
    /* チップを初期化 */
    printf("Initializing OPL3 chip...\n");
    OPL3_Reset(&chip, 49716);  /* サンプリングレート 49716 Hz */
    printf("Chip initialized.\n\n");
    
    /* テストトーンを設定 */
    setup_test_tone();
    
    printf("\nGenerating samples...\n");
    printf("Format: LEFT,RIGHT\n");
    printf("-------------------\n");
    
    /* サンプルを生成して表示 */
    for (i = 0; i < 50; i++) {
        OPL3_GenerateStream(&chip, samples, 1);
        printf("%d,%d\n", samples[0], samples[1]);
    }
    
    printf("-------------------\n");
    
    /* トーンを停止 */
    stop_test_tone();
    
    /* さらにいくつかサンプルを生成(リリースを確認) */
    printf("\nAfter key off:\n");
    for (i = 0; i < 10; i++) {
        OPL3_GenerateStream(&chip, samples, 1);
        printf("%d,%d\n", samples[0], samples[1]);
    }
    
    printf("\nTest completed successfully!\n");
    
#ifdef __SPECTRUM__
    /* ZX Spectrumの場合はキー待ち */
    printf("\nPress any key to exit...");
    getchar();
#endif
    
    return 0;
}
