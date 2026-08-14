#include "common.h"

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;

void *memset(void *buf, char c, size_t n){
	uint8_t *p = (uint8_t *) buf;
	while (n--)
		*p++ = c;
	return buf;
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