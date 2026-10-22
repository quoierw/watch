#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "main.h"
// #define CS_BIT
#define SOFTWARE_SPI 0



#define LED_Pin                                     GPIO_PIN_15
#define LED_GPIO_Port                               GPIOB
#define LCD_SCK_Pin                                 GPIO_PIN_3
#define LCD_SCK_GPIO_Port                           GPIOB
#define LCD_MOSI_Pin                                GPIO_PIN_5
#define LCD_MOSI_GPIO_Port                          GPIOB
#define LCD_RST_Pin                                 GPIO_PIN_7
#define LCD_RST_GPIO_Port                           GPIOB
#define LCD_CS_Pin                                  GPIO_PIN_8
#define LCD_CS_GPIO_Port                            GPIOB
#define LCD_DC_Pin                                  GPIO_PIN_9
#define LCD_DC_GPIO_Port                            GPIOB
#define LCD_BLK_Pin                                 GPIO_PIN_0
#define LCD_BLK_GPIO_Port                           GPIOB


#define LCD_BLK(x)    HAL_GPIO_WritePin(LCD_BLK_GPIO_Port, LCD_BLK_Pin, ((x) ? GPIO_PIN_SET: GPIO_PIN_RESET))
#define LCD_CS(x)    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, ((x) ? GPIO_PIN_SET: GPIO_PIN_RESET))
#define LCD_SCK(x)    HAL_GPIO_WritePin(LCD_SCK_GPIO_Port, LCD_SCK_Pin, ((x) ? GPIO_PIN_SET: GPIO_PIN_RESET))
#define LCD_MOSI(x)    HAL_GPIO_WritePin(LCD_MOSI_GPIO_Port, LCD_MOSI_Pin, ((x) ? GPIO_PIN_SET: GPIO_PIN_RESET))
#define LCD_DC(x)    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, ((x) ? GPIO_PIN_SET: GPIO_PIN_RESET))
#define LCD_RST(x)    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, ((x) ? GPIO_PIN_SET: GPIO_PIN_RESET))


#define LCD_WIDTH 240
#define LCD_HEIGHT 280
#define LCD_Y_SCREEN_OFFSET 20
#define BLACK   0X0000
#define WHITE   0xFFFF
#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F



void lcd_clearScreen(uint16_t color);

void lcd_init(void);

void lcd_setAddrss(uint16_t x, uint16_t y,uint16_t x_e,uint16_t y_e);

void lcd_writeData(uint8_t data);

void lcd_writeData16(uint16_t data);

void lcd_regConfig(void);


void lcd_openLight(void);

void lcd_setLight(uint8_t dc);

void lcd_closeLight(void);


#endif
