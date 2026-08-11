typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;

extern char __bss[], __bss_end[], __stack_top[];//シンボルのアドレスが知りたいので適当にChar型にしているらしい


/* もしも extern char __bss;にすると
実体を持たない変数であるのに、その中身を参照しようとしてしまう。
→&演算子の付け忘れで予期せぬエラーが生じるから、[]にすることで&をつけずとも先頭アドレスを取得できる
*/

void *memset(void *buf, char c, size_t n){
	uint8_t *p = (uint8_t *) buf;
	while (n--)
		*p++ = c;
	return buf;
}

void kernel_main(void){
	memset(__bss, 0, (size_t) __bss_end - (size_t) __bss);

	for (;;);
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