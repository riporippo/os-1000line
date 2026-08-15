#include "common.h"

/* typedef: 元々ある型を違う名前で使用するためのもの */
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;

void *memcpy(void *dst, const void *src, size_t n){
	uint8_t *d = (uint8_t *) dst;
	const uint8_t *s = (const uint8_t *) src;
	while (n--)
	/*
		*d = *s;
		d = d + 1;
		s = s + 1;
	*/
		*d++ = *s++;
	return dst;
}

void *memset(void *buf, char c, size_t n){
	uint8_t *p = (uint8_t *) buf;
	while (n--)
	/*
		*p = c;
		p = p + 1;
	*/
		*p++ = c;
	return buf;
}

char *strcpy_s(char *dst, size_t n, const char *src){//でかすぎて 溢れちゃうなら 切りたもう
	char *d = dst;
	size_t ava = n;
	while(*src && ava > 1){
		*d++ = *src++;
		ava--;
	}
	*d = '\0';
	return dst;
}

int strcmp(const char *s1, const char *s2){
	while(*s1 && *s2){
		if (*s1 != *s2)
			break;
		s1++;
		s2++;
	}
	/* 文字コード同士で引き算することで，辞書順かどうかを判定できるという寸法(0なら完全一致) */
	/* !strcmp(s1,s2) -> True */
	return *(unsigned char *)s1 - *(unsigned char *)s2;
}

void putchar(char ch);

void printf(const char *fmt, ...){
	va_list vargs;//可変長引数のポインタを作成
	va_start(vargs, fmt);// a0: fmt, a1にvargsというポインタを設定

	while (*fmt){
		if (*fmt == '%'){
			fmt++;
			switch(*fmt){
				case '\0':
					putchar('%');
					goto end;
				case '%':
					putchar('%');
					break;
				case 's':{// 文字列
					const char *s = va_arg(vargs, const char *);// vargsが指し示す場所から char型の文字のリストのポインタを取り出し、vargsを1進める
					while (*s){
						putchar(*s);
						s++;
					}
					break;
				}
				case 'd': {// 10進数
					int value = va_arg(vargs, int);// int型でデータを取り出し、vargsを1進める
					unsigned magnitude = value;//unsigned: 符号なし32bit整数の省略形
					if (value < 0){
						putchar('-');
						magnitude = -magnitude;
					}

					unsigned divisor = 1;
					while (magnitude / divisor > 9)
						divisor *= 10;

					while (divisor > 0){
						putchar('0' + magnitude / divisor);
						magnitude %= divisor;
						divisor /= 10;
					}

					break;
				}
				case 'x': {// 16進数
					unsigned value = va_arg(vargs, unsigned);//unsigned int でデータを取り出しvargsを1進める。
					for (int i = 7; i >= 0; i--){//i*4bt右シフト -> ビットマスク 
						unsigned nibble = (value >> (i * 4)) & 0xf;
						putchar("0123456789abcdef"[nibble]);
					}
				}
			}
		} else {
			putchar(*fmt);	
		}

		fmt++;
	}

	end:
		va_end(vargs);
}