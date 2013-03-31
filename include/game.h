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
	int hp;//可以是不同的血量
	int step;//往这个方向还要走多少步
LINKLIST_DEF_FI(enemy)

/* 定义主要角色 */
struct mc{
	int x,y;
	int di;//方向 0左1右2上3下
}ME;

/*定义主要角色和敌方的的子弹*/
LINKLIST_DEF(bullet)
	int x,y;
	int vx,vy;
LINKLIST_DEF_FI(bullet)

/* 主循环 */
void main_loop(void);

/* 游戏逻辑相关 */
void create_main_character(void);
void create_new_enemy(void);
void create_new_enemyb(void);//敌人的子弹用自动生成的方式
void update_enemy_pos(void);
void update_my_pos(void);
void update_mcb_pos(void);
void update_enemyb_pos(void);
bool update_keypress(void);
bool isgameover(void);
int get_hit(void);//击中数
int get_hp(void);//血量
int get_fps(void);
void set_fps(int fps);
enemy_t enemies(void);
bullet_t mcbullets(void);
bullet_t enemybullets(void);

void redraw_screen(void);
void draw_gameover(void);
/* 随机数 */
int rand(void);
void srand(int seed);

#endif
