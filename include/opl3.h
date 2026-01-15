/*
 * Copyright (C) 2013-2020 Alexey Khokholov (Nuke.YKT)
 *
 * This file is part of Nuked OPL3.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 *  Nuked OPL3 emulator.
 *  Thanks:
 *      MAME Development Team(Jarek Burczynski, Tatsuyuki Satoh):
 *          Feedback and Rhythm part calculation information.
 *      forums.submarine.org.uk(carbon14, opl3):
 *          Tremolo and phase generator calculation information.
 *      OPLx decapsulated(Matthew Gambrell, Olli Niemitalo):
 *          OPL2 ROMs.
 *      siliconpr0n.org(John McMaster, digshadow):
 *          YMF262 and VRC VII decaps and die shots.
 *
 * z88dk port version
 */

#ifndef OPL3_H
#define OPL3_H

/* z88dk compatibility */
#ifdef __Z88DK__

/* z88dkでは標準整数型が使えないため、手動定義 */
typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef signed short int16_t;
typedef unsigned long uint32_t;
typedef signed long int32_t;

/* z88dkは64bit整数をサポートしないため、構造体で代替 */
typedef struct {
    uint32_t low;
    uint32_t high;
} uint64_t;

#else
/* 通常のコンパイラでは標準ヘッダーを使用 */
#include <stdint.h>
#endif

/* OPL3チップの状態を保持する構造体 */
typedef struct _opl3_slot opl3_slot;
typedef struct _opl3_channel opl3_channel;
typedef struct _opl3_chip opl3_chip;

/* スロット(オペレータ)の状態 */
struct _opl3_slot {
    opl3_channel *channel;
    opl3_chip *chip;
    int16_t out;
    int16_t fbmod;
    int16_t *mod;
    int16_t prout;
    uint16_t eg_rout;
    uint16_t eg_out;
    uint8_t eg_inc;
    uint8_t eg_gen;
    uint8_t eg_rate;
    uint8_t eg_ksl;
    uint8_t *trem;
    uint8_t reg_vib;
    uint8_t reg_type;
    uint8_t reg_ksr;
    uint8_t reg_mult;
    uint8_t reg_ksl;
    uint8_t reg_tl;
    uint8_t reg_ar;
    uint8_t reg_dr;
    uint8_t reg_sl;
    uint8_t reg_rr;
    uint8_t reg_wf;
    uint8_t key;
    uint32_t pg_reset;
    uint32_t pg_phase;
    uint16_t pg_phase_out;
    uint8_t slot_num;
};

/* チャンネルの状態 */
struct _opl3_channel {
    opl3_slot *slots[2];
    opl3_channel *pair;
    opl3_chip *chip;
    int16_t *out[4];
    uint8_t chtype;
    uint16_t f_num;
    uint8_t block;
    uint8_t fb;
    uint8_t con;
    uint8_t alg;
    uint8_t ksv;
    uint16_t cha, chb;
    uint8_t ch_num;
};

/* OPL3チップ全体の状態 */
struct _opl3_chip {
    opl3_channel channel[18];
    opl3_slot slot[36];
    uint16_t timer;
    uint64_t eg_timer;
    uint8_t eg_timerrem;
    uint8_t eg_state;
    uint8_t eg_add;
    uint8_t eg_timer_lo;
    uint8_t newm;
    uint8_t nts;
    uint8_t rhy;
    uint8_t vibpos;
    uint8_t vibshift;
    uint8_t tremolo;
    uint8_t tremolopos;
    uint8_t tremoloshift;
    uint32_t noise;
    int16_t zeromod;
    int32_t mixbuff[4];
    uint8_t rm_hh_bit2;
    uint8_t rm_hh_bit3;
    uint8_t rm_hh_bit7;
    uint8_t rm_hh_bit8;
    uint8_t rm_tc_bit3;
    uint8_t rm_tc_bit5;
    /* OPL3L */
    int32_t rateratio;
    int32_t samplecnt;
    int16_t oldsamples[2];
    int16_t samples[2];
};

/* 関数プロトタイプ */

/* チップの初期化とリセット */
void OPL3_Reset(opl3_chip *chip, uint32_t samplerate);

/* レジスタへの書き込み */
void OPL3_WriteReg(opl3_chip *chip, uint16_t reg, uint8_t v);
void OPL3_WriteRegBuffered(opl3_chip *chip, uint16_t reg, uint8_t v);

/* サンプル生成(複数フォーマット対応) */
void OPL3_Generate(opl3_chip *chip, int16_t *buf);
void OPL3_GenerateResampled(opl3_chip *chip, int16_t *buf);

/* ステレオストリーム生成 */
void OPL3_GenerateStream(opl3_chip *chip, int16_t *sndptr, uint32_t numsamples);

/* z88dk最適化用のマクロ */
#ifdef __Z88DK__
/* インライン展開を積極的に行う(小さい関数のみ) */
#define OPL3_INLINE static inline

/* ROMに配置するデータ用 */
#define OPL3_CONST const

/* アセンブラ最適化が必要な場合のヒント */
#define OPL3_FASTCALL __z88dk_fastcall

#else
#define OPL3_INLINE static inline
#define OPL3_CONST const
#define OPL3_FASTCALL
#endif

#endif /* OPL3_H */
