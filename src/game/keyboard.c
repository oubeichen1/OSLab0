#include "common.h"
#include "string.h"

/* 按键对应的键盘扫描码 */
static int letter_code[] = {
	75,72,77,80,57//左、上、右、下、空格，这样排序是为了方便找到反方向
};
/* 对应键按下的标志位 */
static bool letter_pressed[5];

void
press_key(int scan_code) {
	int i;
	for (i = 0; i < 5; i ++) {
		if (letter_code[i] == scan_code) {
			letter_pressed[i] = TRUE;
		}
	}
}

void
release_key(int index) {
	assert(0 <= index && index < 5);
	letter_pressed[index] = FALSE;
}

bool
query_key(int index) {
	assert(0 <= index && index < 5);
	return letter_pressed[index];
}

static volatile int key_code = 0;

void
keyboard_event(int code) {
	key_code = code;
	press_key(code);
}

