#include "kernel.h"
#include "common.h"

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;

/* もしも extern char __bss;にすると
実体を持たない変数であるのに、その中身を参照しようとしてしまう。
→&演算子の付け忘れで予期せぬエラーが生じるから、[]にすることで&をつけずとも先頭アドレスを取得できる
*/

extern char __bss[], __bss_end[], __stack_top[];//シンボルのアドレスが知りたいので適当にChar型にしているらしい

struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4, long arg5, long fid, long eid) {
	/* 指定したレジスタに値を入れる */
	register long a0 __asm__("a0") = arg0;
	register long a1 __asm__("a1") = arg1;
	register long a2 __asm__("a2") = arg2;
	register long a3 __asm__("a3") = arg3;
	register long a4 __asm__("a4") = arg4;
	register long a5 __asm__("a5") = arg5;
	register long a6 __asm__("a6") = fid;
	register long a7 __asm__("a7") = eid;

	__asm__ __volatile__("ecall"
		                : "=r"(a0), "=r"(a1)
						: "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5), "r"(a6), "r"(a7)
						: "memory");
	/* CPUの権限レベル
	- M Mode: OpenSBI
	- S Mode: Kernel
	- U Mode: Application
	
	ecall命令とは
	- M Modeのトラップハンドラ(レジスタ: mtvec)へジャンプする命令(今回の場合)
	- U Mode から S Modeを呼び出す(システムコール)にも使われる。

	SBIの呼び出し規約
	- a7, a6: eid, fid
	- a0 - a5: 引数
	- 結果は a0, a1 に入れて返す

	eid: SBI extension ID -> 実行ID
	fid: SBI function ID -> 実行IDに紐づいた具体的な処理を表す関数ID？
	*/
	return (struct sbiret){.error = a0, .value = a1};
}

void putchar(char ch){
	sbi_call(ch, 0,0,0,0,0,0,1);/* Console Putchar機能を用いるという意味 */
}

void kernel_main(void){
	printf("\n\nHello %s\n", "World!");
	printf("1 + 2 = %d, %x\n", 1 + 2, 0x1234abcd);

	for (;;){
		__asm__ __volatile__("wfi");
	}
}

__attribute__((section(".text.boot")))
__attribute__((naked)) // 関数冒頭、末尾のコードを書かないということ(boot関数より前の関数は存在しない→戻り先が存在せずパニックになるため)
void boot(void){
	__asm__ __volatile__(
		"mv sp, %[stack_top]\n" //spにフレームポインタを代入
		"j kernel_main\n" //問答無用でkernel_mainに遷移
		:
		: [stack_top] "r" (__stack_top) //リンカで定義されたフレームポインタ
	);
}