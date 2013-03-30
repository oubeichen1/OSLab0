#ifndef __GAME_H__
#define __GAME_H__

#include "common.h"
#include "adt/linklist.h"

/* 中断时调用的函数 */
void timer_event(void);
void keyboard_event(int scan_code);

/* 按键相关 */
void press_key(int scan_code);
void release_key(int ch);
bool query_key(int ch);
int last_key_code(void);
/*定义角色大小*/
#define SIZE_OF_CHARACTER 16

/* 定义敌人链表 */
LINKLIST_DEF(enemy)
	int x, y;//整个游戏只有整数坐标，便于设计
	int di;
	bool dead;//是否死亡
	int step;//往这个方向还要走多少步
LINKLIST_DEF_FI(enemy)

/* 定义主要角色 */
struct mc{
	int x,y;
	int di;//方向 0左1右2上3下
}ME;

/*定义主要角色的子弹*/
LINKLIST_DEF(mcb)
	int x,y;
	int vx,vy;
LINKLIST_DEF_FI(mcb)

/* 主循环 */
void main_loop(void);

/* 游戏逻辑相关 */
void create_main_character(void);
void create_new_enemy(void);
void update_enemy_pos(void);
void update_my_pos(void);
void update_mcb_pos(void);
bool update_keypress(void);

int get_hit(void);
int get_miss(void);
int get_fps(void);
void set_fps(int fps);
enemy_t enemies(void);
mcb_t mcbullets(void);

void redraw_screen(void);

/* 随机数 */
int rand(void);
void srand(int seed);

#endif
