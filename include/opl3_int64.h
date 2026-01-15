/*
 * 64-bit integer operations for z88dk
 * 
 * z88dkは64bit整数をネイティブサポートしないため、
 * 32bitペアで実装します
 */

#ifndef OPL3_INT64_H
#define OPL3_INT64_H

#include "opl3.h"

#ifdef __Z88DK__

/* 64bit値の初期化 */
static inline void uint64_init(uint64_t* val, uint32_t low, uint32_t high) {
    val->low = low;
    val->high = high;
}

/* 64bit値をゼロにセット */
static inline void uint64_zero(uint64_t* val) {
    val->low = 0;
    val->high = 0;
}

/* 64bit加算: result = a + b */
static inline void uint64_add(uint64_t* result, const uint64_t* a, const uint64_t* b) {
    uint32_t low = a->low + b->low;
    result->high = a->high + b->high;
    /* キャリーのチェック */
    if (low < a->low) {
        result->high++;
    }
    result->low = low;
}

/* 64bit加算(32bit値): result = a + b */
static inline void uint64_add32(uint64_t* result, const uint64_t* a, uint32_t b) {
    uint32_t low = a->low + b;
    result->high = a->high;
    if (low < a->low) {
        result->high++;
    }
    result->low = low;
}

/* 64bit減算: result = a - b */
static inline void uint64_sub(uint64_t* result, const uint64_t* a, const uint64_t* b) {
    uint32_t low = a->low - b->low;
    result->high = a->high - b->high;
    /* ボローのチェック */
    if (a->low < b->low) {
        result->high--;
    }
    result->low = low;
}

/* 64bit左シフト: result = val << shift (shift < 32) */
static inline void uint64_shl(uint64_t* result, const uint64_t* val, uint8_t shift) {
    if (shift == 0) {
        *result = *val;
        return;
    }
    if (shift >= 32) {
        result->high = val->low << (shift - 32);
        result->low = 0;
        return;
    }
    result->high = (val->high << shift) | (val->low >> (32 - shift));
    result->low = val->low << shift;
}

/* 64bit右シフト: result = val >> shift (shift < 32) */
static inline void uint64_shr(uint64_t* result, const uint64_t* val, uint8_t shift) {
    if (shift == 0) {
        *result = *val;
        return;
    }
    if (shift >= 32) {
        result->low = val->high >> (shift - 32);
        result->high = 0;
        return;
    }
    result->low = (val->low >> shift) | (val->high << (32 - shift));
    result->high = val->high >> shift;
}

/* 64bit比較: a < b なら1を返す */
static inline uint8_t uint64_less(const uint64_t* a, const uint64_t* b) {
    if (a->high < b->high) return 1;
    if (a->high > b->high) return 0;
    return a->low < b->low;
}

/* 64bit比較: a == b なら1を返す */
static inline uint8_t uint64_equal(const uint64_t* a, const uint64_t* b) {
    return (a->high == b->high) && (a->low == b->low);
}

/* 64bitビットAND: result = a & b */
static inline void uint64_and(uint64_t* result, const uint64_t* a, const uint64_t* b) {
    result->low = a->low & b->low;
    result->high = a->high & b->high;
}

/* 64bitビットOR: result = a | b */
static inline void uint64_or(uint64_t* result, const uint64_t* a, const uint64_t* b) {
    result->low = a->low | b->low;
    result->high = a->high | b->high;
}

/* 64bit値を32bit値に変換(下位32bitのみ) */
static inline uint32_t uint64_to_uint32(const uint64_t* val) {
    return val->low;
}

/* 32bit値を64bit値に変換 */
static inline void uint32_to_uint64(uint64_t* result, uint32_t val) {
    result->low = val;
    result->high = 0;
}

/* 64bitインクリメント: val++ */
static inline void uint64_inc(uint64_t* val) {
    val->low++;
    if (val->low == 0) {
        val->high++;
    }
}

#else
/* 通常のコンパイラでは標準の演算を使用 */
#define uint64_init(v, l, h) (*(v) = ((uint64_t)(h) << 32) | (l))
#define uint64_zero(v) (*(v) = 0)
#define uint64_add(r, a, b) (*(r) = *(a) + *(b))
#define uint64_add32(r, a, b) (*(r) = *(a) + (b))
#define uint64_sub(r, a, b) (*(r) = *(a) - *(b))
#define uint64_shl(r, v, s) (*(r) = *(v) << (s))
#define uint64_shr(r, v, s) (*(r) = *(v) >> (s))
#define uint64_less(a, b) (*(a) < *(b))
#define uint64_equal(a, b) (*(a) == *(b))
#define uint64_and(r, a, b) (*(r) = *(a) & *(b))
#define uint64_or(r, a, b) (*(r) = *(a) | *(b))
#define uint64_to_uint32(v) ((uint32_t)(*(v)))
#define uint32_to_uint64(r, v) (*(r) = (v))
#define uint64_inc(v) ((*(v))++)
#endif

#endif /* OPL3_INT64_H */
