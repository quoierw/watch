#ifndef __LCD_H
#define __LCD_H

#include "stm32f4xx_hal.h"

typedef void (* lcd_callback_t)(void);

void lcd_fill(uint16_t x, uint16_t y, uint16_t x_e, uint16_t y_e, uint16_t color);
void lcd_draw_point(uint16_t x, uint16_t y,uint16_t color);
void lcd_draw_line(uint16_t x, uint16_t y, uint16_t x_e, uint16_t y_e, uint16_t color);
void lcd_draw_rectangle(uint16_t x, uint16_t y, uint16_t x_e, uint16_t y_e, uint16_t color);
void lcd_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);

void lcd_show_char(uint16_t x, uint16_t y, uint8_t showChar, uint16_t bc, uint16_t fc, uint8_t sizey, uint8_t mode);
void lcd_show_string(uint16_t x, uint16_t y, const uint8_t *str, uint16_t bc, uint16_t fc, uint8_t sizey, uint8_t mode);

void lcd_show_chinese12x12(uint16_t x, uint16_t y, uint8_t * showChar, uint16_t bc, uint16_t fc, uint8_t sizey, uint8_t mode);
void lcd_show_chinese16x16(uint16_t x, uint16_t y, uint8_t * showChar, uint16_t bc, uint16_t fc, uint8_t sizey, uint8_t mode);
void lcd_show_chinese24x24(uint16_t x, uint16_t y, uint8_t * showChar, uint16_t bc, uint16_t fc, uint8_t sizey, uint8_t mode);
void lcd_show_chinese32x32(uint16_t x, uint16_t y, uint8_t * showChar, uint16_t bc, uint16_t fc, uint8_t sizey, uint8_t mode);
void lcd_show_intNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t bc, uint16_t fc, uint8_t sizey);
void lcd_show_floatNum(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t bc, uint16_t fc, uint8_t sizey);

void lcd_color_fill(uint16_t x, uint16_t y, uint16_t x_e, uint16_t y_e, uint16_t* color);

void lcd_set_flush_complete_callback(lcd_callback_t cb);


#endif
