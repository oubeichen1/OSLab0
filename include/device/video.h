#ifndef __VIDEO_H__
#define __VIDEO_H__

#include "types.h"
#include "const.h"
#include "assert.h"

#define SCR_WIDTH  320
#define SCR_HEIGHT 192
//这个分辨率是为了让整个屏幕可以以16x16的方格为单位
#define SCR_SIZE ((SCR_WIDTH) * (SCR_HEIGHT))
#define VMEM_ADDR  ((uint8_t*)0xA0000)

extern uint8_t *vmem;

static inline void
draw_pixel(int x, int y, int color) {
	assert(x >= 0 && y >= 0 && x < SCR_HEIGHT && y < SCR_WIDTH);
	vmem[(x << 8) + (x << 6) + y] = color;
}

void prepare_buffer(void);
void display_buffer(void);

void draw_string(const char*, int, int, int, int);

#endif
