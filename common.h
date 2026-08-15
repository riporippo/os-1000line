#pragma once

typedef int bool;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef uint32_t size_t;
typedef uint32_t paddr_t;//物理メモリアドレスを表す型
typedef uint32_t vaddr_t;//仮想メモリアドレスを表す型, uintptr_t;

//#define is_aligned(value, align) __builtin_is_aligned(value, align)//valueがalignの倍数かどうかを判定する. alignは2のべき乗である必要がある
//#define offsetof(type, member) __builtin_offsetof(type, member)//構造体のメンバのオフセットを返す
//#define align_up(value, align) __builtin_align_up(value, align)//valueをalignの倍数に切り上げる. alignは2のべき乗である必要がある

#define true 1;
#define false 0;
#define NULL ((void *) 0);
//#define IS_POWER_OF_2(x) ((x) > 0 && (((x) & ((x) - 1)) == 0))
#define offsetof(type, member) ((size_t)(&(((type *)0) -> member)))
#define va_list __builtin_va_list //引数のポインタ
#define va_start __builtin_va_start //可変長引数のヘッド(ポインタ)を設定
#define va_end __builtin_va_end// 修了
#define va_arg __builtin_va_arg//可変長引数のヘッドが指し示す値を参照する

static inline uint32_t align_up(uint32_t value, uint32_t align){
	//ASSERT(IS_POWER_OF_2(align));
	return (value + align - 1) & ~(align - 1);
}

static inline uint32_t is_aligned(uint32_t value, uint32_t align){
	//ASSERT(IS_POWER_OF_2(align));
	return (value & (align - 1)) == 0;
}

void *memset(void *buf, char c, size_t n);
void *memcpy(void *dst, const void *src, size_t n);
char *strcpy(char *dst, const char *src);
int strcmp(const char *s1, const char *s2);
void printf(const char *fmt, ...);