/*
 * Musical scale example for Nuked-OPL3 on z88dk
 * 
 * ド・レ・ミ・ファ・ソ・ラ・シ・ドの音階を演奏します
 */

#include <stdio.h>
#include "opl3.h"

#ifdef __Z88DK__
#include <arch.h>
#endif

static opl3_chip chip;

/* 音符のF-Number値(A4=440Hzを基準) */
static const uint16_t note_fnum[] = {
    0x156,  /* C4 (ド)   261.63 Hz */
    0x16B,  /* D4 (レ)   293.66 Hz */
    0x181,  /* E4 (ミ)   329.63 Hz */
    0x198,  /* F4 (ファ) 349.23 Hz */
    0x1B0,  /* G4 (ソ)   392.00 Hz */
    0x1CA,  /* A4 (ラ)   440.00 Hz */
    0x1E5,  /* B4 (シ)   493.88 Hz */
    0x202   /* C5 (ド)   523.25 Hz */
};

/* 音符名 */
static const char* note_names[] = {
    "C4 (Do)", "D4 (Re)", "E4 (Mi)", "F4 (Fa)",
    "G4 (So)", "A4 (La)", "B4 (Si)", "C5 (Do)"
};

/* 簡易ディレイ(Z80向け) */
void delay(uint16_t count) {
    volatile uint16_t i, j;
    for (i = 0; i < count; i++) {
        for (j = 0; j < 100; j++) {
            /* 何もしない */
        }
    }
}

/* OPL3を初期化して楽器を設定 */
void setup_instrument(void) {
    /* チップをリセット */
    OPL3_Reset(&chip, 49716);
    
    /* OPL3モードを有効化 */
    OPL3_WriteReg(&chip, 0x105, 0x01);
    OPL3_WriteReg(&chip, 0x104, 0x00);
    
    /* ピアノ風の音色を設定 */
    /* オペレータ1(キャリア) */
    OPL3_WriteReg(&chip, 0x20, 0x21);   /* MULT=1, KSR=1 */
    OPL3_WriteReg(&chip, 0x40, 0x0A);   /* TL=10 */
    OPL3_WriteReg(&chip, 0x60, 0xF4);   /* AR=15, DR=4 */
    OPL3_WriteReg(&chip, 0x80, 0x55);   /* SL=5, RR=5 */
    OPL3_WriteReg(&chip, 0xE0, 0x00);   /* WS=0 */
    
    /* オペレータ0(モジュレータ) */
    OPL3_WriteReg(&chip, 0x23, 0x01);   /* MULT=1 */
    OPL3_WriteReg(&chip, 0x43, 0x00);   /* TL=0 */
    OPL3_WriteReg(&chip, 0x63, 0xF2);   /* AR=15, DR=2 */
    OPL3_WriteReg(&chip, 0x83, 0x53);   /* SL=5, RR=3 */
    OPL3_WriteReg(&chip, 0xE3, 0x00);   /* WS=0 */
    
    /* チャンネル設定 */
    OPL3_WriteReg(&chip, 0xC0, 0x01);   /* FB=0, ALG=1 */
}

/* 音符を演奏 */
void play_note(uint8_t note_index) {
    uint16_t fnum;
    uint8_t block;
    
    if (note_index >= 8) return;
    
    /* F-Numberとブロック番号を取得 */
    fnum = note_fnum[note_index];
    block = 4;  /* Block 4 */
    
    printf("Playing: %s\n", note_names[note_index]);
    
    /* 周波数を設定 */
    OPL3_WriteReg(&chip, 0xA0, fnum & 0xFF);
    OPL3_WriteReg(&chip, 0xB0, 0x20 | (block << 2) | ((fnum >> 8) & 0x03));
    
    /* Key ON */
    OPL3_WriteReg(&chip, 0xB0, 0x20 | (block << 2) | ((fnum >> 8) & 0x03) | 0x20);
    
    /* 音を鳴らす時間(サンプル生成) */
    {
        int16_t samples[2];
        uint16_t i;
        for (i = 0; i < 12000; i++) {  /* 約0.24秒 */
            OPL3_GenerateStream(&chip, samples, 1);
        }
    }
    
    /* Key OFF */
    OPL3_WriteReg(&chip, 0xB0, 0x00 | (block << 2) | ((fnum >> 8) & 0x03));
    
    /* リリース時間 */
    {
        int16_t samples[2];
        uint16_t i;
        for (i = 0; i < 5000; i++) {  /* 約0.1秒 */
            OPL3_GenerateStream(&chip, samples, 1);
        }
    }
}

/* メイン関数 */
int main(void) {
    uint8_t i;
    
    printf("Nuked-OPL3 Musical Scale Demo\n");
    printf("==============================\n\n");
    
    printf("Initializing OPL3...\n");
    setup_instrument();
    printf("Ready!\n\n");
    
    printf("Playing C major scale:\n");
    printf("----------------------\n");
    
    /* ドレミファソラシドを演奏 */
    for (i = 0; i < 8; i++) {
        play_note(i);
    }
    
    printf("----------------------\n");
    printf("Scale complete!\n\n");
    
    /* 和音のデモ(簡易版) */
    printf("Playing chord (C-E-G):\n");
    
    /* 3つの音を同時に設定(マルチチャンネル) */
    /* ここでは簡略化のため省略 */
    
#ifdef __SPECTRUM__
    printf("\nPress any key to exit...");
    getchar();
#endif
    
    return 0;
}
