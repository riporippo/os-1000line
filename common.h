#pragma once

#define va_list __builtin_va_list //引数のポインタ
#define va_start __builtin_va_start //可変長引数のヘッド(ポインタ)を設定
#define va_end __builtin_va_end
#define va_arg __builtin_va_arg// 

void printf(const char *fmt, ...);