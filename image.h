#ifndef __IMAGE_H__
#define __IMAGE_H__
#include "lcd.h"

#define HEIGHT LCDWIDTH
#define WIDTH  LCDHEIGHT

typedef struct  {
	double left, top;
	double width, height;
	uint16_t colour;
 } image;

#define SMALL_HEIGHT 16
#define SMALL_WIDTH  12

#define BIG_HEIGHT 32
#define BIG_WIDTH 24


void fill_image(image i, uint16_t colour); 
void draw_image_indexed(image i, uint16_t* colour);
void draw_image_indexed_8(image i, uint8_t* colour);

#endif
