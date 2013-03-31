#include "game.h"
#include "string.h"
#include "device/video.h"

/* 绘制屏幕上的内容。
 * 注意程序在绘图之前调用了prepare_buffer，结束前调用了display_buffer。
 * prepare_buffer会准备一个空白的绘图缓冲区，display_buffer则会将缓冲区绘制到屏幕上，
 * draw_pixel或draw_string绘制的内容将保存在缓冲区内(暂时不会显示在屏幕上)，调用
 * display_buffer后才会显示。
*/
void
redraw_screen() {
	enemy_t enemyit;
	bullet_t it;
	const char *hit,*hp;
	
	prepare_buffer(); /* 准备缓冲区 */

	static char buf[2];
	/*绘制主角子弹*/
	for(it = mcbullets();it != NULL;it = it->_next){
		buf[0] = 5;
		draw_string(buf,it->x,it->y,15,SIZE_OF_CHARACTER);
	}
	/*绘制敌方子弹*/
	for(it = enemybullets();it != NULL;it = it->_next){
		buf[0] = 5;
		draw_string(buf,it->x,it->y,14,SIZE_OF_CHARACTER);
	}
	/* 绘制每个敌方坦克 */
	for (enemyit = enemies(); enemyit != NULL; enemyit = enemyit->_next) {
		if(enemyit->hp == 0)
			buf[0] = 6;//被击毁的样式
		else
			buf[0] = enemyit->di+1; 
		buf[1] = 0;
		draw_string(buf, enemyit->x, enemyit->y,15 - enemyit->hp,SIZE_OF_CHARACTER);
	}
	/*绘制主角*/
	buf[0] = ME.di+1;
	buf[1] = 0;
	draw_string(buf,ME.x,ME.y,15,SIZE_OF_CHARACTER);
	/* 绘制命中数、hp数、fps */
	//draw_string(itoa(last_key_code()), SCR_HEIGHT - 8, 0, 48);
	hit = itoa(get_hit());
	draw_string(hit, 0, SCR_WIDTH - strlen(hit) * 8, 10,8);
	hp = itoa(get_hp());
	draw_string("HP", SCR_HEIGHT - 8, SCR_WIDTH - 32,12,8);
	draw_string(hp, SCR_HEIGHT - 8, SCR_WIDTH - strlen(hp) * 8, 12,8);
	draw_string(itoa(get_fps()), 0, 0, 14,8);
	draw_string("FPS", 0, strlen(itoa(get_fps())) * 8, 14,8);

	display_buffer(); /* 绘制缓冲区 */
}
void draw_gameover()
{
	prepare_buffer();
	draw_string("G A M E O V E R !",SCR_HEIGHT/2- 8,SCR_WIDTH/2 - 75,10,16);
	draw_string("Press SPACE to exit...",SCR_HEIGHT - 8,SCR_WIDTH/2 - 100,10,8);
	display_buffer();
}
