#include"game.h"
static int seed = 0;

void
srand(int s) {
	seed = s;
}

/* 生成下一个随机数 */
int
rand(void) {
	seed = 0x015A4E35 * seed + 1;
	return ((seed >> 16) + last_key_code()) & 0x7FFF;//根据上次按键来随机，更加随机一点
}

