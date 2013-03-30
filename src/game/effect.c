#include "game.h"
#include "common.h"
#include "string.h"
#include "adt/linklist.h"
#include "device/video.h"
#include "x86/x86.h"

LINKLIST_IMPL(enemy, 10000)
LINKLIST_IMPL(mcb, 10000)

static enemy_t enemyhead = NULL;
static mcb_t mcbhead = NULL;
static int hit = 0, miss = 0;
static int cannotshoot = 0;//防止射速过快
const int vx[4] = {0,-SIZE_OF_CHARACTER,0,SIZE_OF_CHARACTER},vy[4] = {-SIZE_OF_CHARACTER,0,SIZE_OF_CHARACTER,0};//左上右下四种方向的位移

int
get_hit(void) {
	return hit;
}

int
get_miss(void) {
	return miss;
}
enemy_t
enemies(void) {
	return enemyhead;
}
mcb_t
mcbullets(void){
	return mcbhead;
}
/* 在屏幕上创建主要角色 */
void
create_main_character(void){
	ME.x = SCR_HEIGHT / 2;
	ME.y = SCR_WIDTH / 2;
	ME.di = 2;
}
/* 在屏幕上创建一个新的敌人 */
void
create_new_enemy(void) {
	if (enemyhead == NULL) {
		enemyhead = enemy_new(); /* 当前没有任何字母，创建新链表 */
	} else {
		enemy_t now = enemy_new();
		enemy_insert(NULL, enemyhead, now); /* 插入到链表的头部 */
		enemyhead = now;
	}
	/* 敌人的初始位置随机设定 */
	enemyhead->x = rand()%2?0:SCR_HEIGHT-SIZE_OF_CHARACTER;
	enemyhead->y = rand()%2?0:SCR_WIDTH-SIZE_OF_CHARACTER;
	//head->v = (rand() % 1000 / 1000.0 + 1) / 2.0;
	enemyhead->di = rand()%4;//初始方向随机，不过马上就会变成朝向主角移动
	enemyhead->step = 0;//移动和方向的AI统一在update_enemy_pos里设置
	//release_key(head->text); /* 清除过往的按键 */
}

/* 敌人坦克移动1单位
 * AI是这样设计的：在x或者y方向朝向主角移动。每次要朝一个方向移动随机步数（0～8），然后才能更换方向，否则显得太不自然 */
void
update_enemy_pos(void) {
	enemy_t it,other;
	for (it = enemyhead; it != NULL; ) {
		enemy_t next = it->_next;
		if(it->dead)//坦克已被击毁
		{
			hit++;//击中一架敌方的坦克
			enemy_remove(it);
			enemy_free(it);
			if(it == enemyhead)enemyhead = next;
		}
		else if(it->step > 0)//坦克还要在这个方向上移动
		{
			it->step--;
			it->x += vx[it->di]; /* 更新位置 */
			it->y += vy[it->di];
			if (it->x < 0 || it->x + SIZE_OF_CHARACTER > SCR_HEIGHT || it->y < 0 || it->y + SIZE_OF_CHARACTER > SCR_WIDTH)//触及了边界马上更换反方向,简单一点吧; 
			{
				it->x -= vx[it->di];
				it->y -= vy[it->di];
				it->di = (it->di + 2) % 4;//由于方向代码的顺序是左、上、右、下，所以很容易就能换成反方向
			}
			else if (it->x == ME.x && it->y == ME.y)//遇到主角不更换方向，当然是要进攻的
			{
				it->x -= vx[it->di];
				it->y -= vy[it->di];
			}
			else//触及了其他敌方坦克也要马上更换方向，这次是靠近主角的方向
			{
				for(other = enemyhead;other != NULL;other = other->_next)//一辆辆坦克尝试
				{
					//将可选方向准备好，然后一次性随机选择
					int numofcand = 0;//备选的方向个数
					int cand_direction[2] = {0,0};//如果主角和坦克在同一条线，则只有一个方向，即主角所在方向，如果不在同一个方向，则x方向和y方向各有一个靠近主角的方向可以选择;
					if(other != it && other->x == it->x && other->y == it->y)//被其他敌方坦克挡住
					{
						it->x -= vx[it->di];
						it->y -= vy[it->di];
						if(ME.x < it->x)//上边
							cand_direction[numofcand++] = 1;
						else if(ME.x > it->x)//下边
							cand_direction[numofcand++] = 3;
							//如果主角和坦克x值相同，则只可能是一条线了
						if(ME.y < it->y)//左边
							cand_direction[numofcand++] = 0;
						else if(ME.y > it->y)//右边
							cand_direction[numofcand++] = 2;
							//如果主角和坦克y值相同，则只可能是一条线了
						it->di = cand_direction[rand()%numofcand];
						break;
					}
				}
			}
		}
		else//坦克已经移动了相应步数，应该要更换方向了;
		{
			int numofcand = 0;//备选的方向个数
			int cand_direction[2] = {-1,-1};//如果主角和坦克在同一条线，则只有一个方向，即主角所在方向，如果不在同一个方向，则x方向和y方向各有一个靠近主角的方向可以选择;
			if(ME.x < it->x)//上边
				cand_direction[numofcand++] = 1;
			else if(ME.x > it->x)//下边
				cand_direction[numofcand++] = 3;
				//如果主角和坦克x值相同，则只可能是一条线了
			if(ME.y < it->y)//左边
				cand_direction[numofcand++] = 0;
			else if(ME.y > it->y)//右边
				cand_direction[numofcand++] = 2;
				//如果主角和坦克y值相同，则只可能是一条线了
			assert(numofcand > 0);
			//assert(cand_direction[0] >= 0);//至少有一个选项，但现在主角可以穿越其他坦克，所以暂时不用这个assert
			it->di = cand_direction[rand()%numofcand];
			it->step = rand()%8;
		}
		it = next;//继续更新下一个坦克
	}
}
/*主角的子弹移动1单位*/
void
update_mcb_pos(void){
	if(cannotshoot)cannotshoot--;//子弹移动了1秒后才能射击下一次。
	mcb_t it;
	for(it = mcbhead;it != NULL;){
		mcb_t next = it->_next;
		it->x += it->vx;
		it->y += it->vy;
		if (it->x < 0 || it->x + SIZE_OF_CHARACTER > SCR_HEIGHT || it->y < 0 ||it->y + SIZE_OF_CHARACTER > SCR_WIDTH) {
			mcb_remove(it);
			mcb_free(it);
			if (it == mcbhead) mcbhead = next; /* 更新链表 */
		}
		it = next;
	}
}

/* 根据按键移动主角的位置 */
bool
update_keypress(void) {
	//fly_t it, target = NULL;
	//float min = -100;
	int i;
	disable_interrupt();
	/* 寻找相应键已被按下、最底部且未被击中的字符 
	for (it = head; it != NULL; it = it->_next) {
		assert(it->text >= 0 && it->text < 26);
		if (it->v > 0 && it->x > min && query_key(it->text)) {
			min = it->x;
			target = it;
		}
	}
	//如果找到则更新相应数据 
	if (target != NULL) {
		release_key(target->text);
		target->v = -3;  //速度改为向上 
		return TRUE;
	}*/
	for(i = 0;i < 4;i++)//对应不同的方向键
	{
		if(query_key(i))
		{
			ME.x += vx[i];
			ME.y += vy[i];
			if(ME.x < 0 || ME.x > SCR_HEIGHT - SIZE_OF_CHARACTER)
				ME.x -= vx[i];
			else if(ME.y < 0 || ME.y > SCR_WIDTH - SIZE_OF_CHARACTER)
				ME.y -= vy[i];
			ME.di = i;
			release_key(i);
			return TRUE;
		}
	}

	/*按下了空格键，发射子弹*/
	if(query_key(4)&&cannotshoot == 0)
	{
		cannotshoot = 50;
		if (mcbhead == NULL) {
			mcbhead = mcb_new(); /* 当前没有主角子弹 创建新链表 */
		} else {
			mcb_t now = mcb_new();
			mcb_insert(NULL, mcbhead, now); /* 插入到链表的头部 */
			mcbhead = now;
		}
		/* 字母、初始位置、掉落速度均为随机设定 */
		mcbhead->x = ME.x;
		mcbhead->y = ME.y;
		mcbhead->vx = vx[ME.di] / SIZE_OF_CHARACTER;
		mcbhead->vy = vy[ME.di] / SIZE_OF_CHARACTER;//保持子弹的流畅性，还是一次移动一个单位
		release_key(4);
		return TRUE;
	}

	enable_interrupt();
	return FALSE;
}

