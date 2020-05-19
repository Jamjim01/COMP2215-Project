#include "image.h"

void draw_image_indexed(image i, uint16_t* colour){
    rectangle r;
    
    r.left=i.left;
    r.right=i.left+i.width-1;
    
    r.top=i.top;
    r.bottom=i.top+i.height-1;
    r.bottom=r.bottom>=HEIGHT ? HEIGHT : r.bottom;
    fill_rectangle_indexed(r,colour);
}

void draw_image_indexed_8(image i, uint8_t* colour){
    rectangle r;
    
    r.left=i.left;
    r.right=i.left+i.width-1;
    
    r.top=i.top;
    r.bottom=i.top+i.height-1;
    r.bottom=r.bottom>=HEIGHT ? HEIGHT : r.bottom;
    fill_rectangle_indexed(r,colour);
}