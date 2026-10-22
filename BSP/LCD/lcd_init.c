#include "lcd_init.h"
#include "tim.h"
#include "spi.h"

void lcd_gpio_init(void)
{
  #if SOFTWARE_SPI 
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_BLK_Pin|LCD_SCK_Pin|LCD_MOSI_Pin
                          |LCD_RST_Pin|LCD_CS_Pin|LCD_DC_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : LCD_BLK_Pin LED_Pin LCD_CLK_Pin LCD_MOSI_Pin
                           LCD_RST_Pin LCD_CS_Pin LCD_DC_Pin */
  GPIO_InitStruct.Pin = LCD_BLK_Pin|LCD_SCK_Pin|LCD_MOSI_Pin
                          |LCD_RST_Pin|LCD_CS_Pin|LCD_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  //GPIO_SPEED_FREQ_LOW
  LCD_CS(0);
  #elif !SOFTWARE_SPI
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_RST_Pin|LCD_CS_Pin|LCD_DC_Pin, GPIO_PIN_SET);
                          
  /*Configure GPIO pins : LCD_BLK_Pin LED_Pin LCD_CLK_Pin LCD_MOSI_Pin
                           LCD_RST_Pin LCD_CS_Pin LCD_DC_Pin */
  GPIO_InitStruct.Pin = LCD_RST_Pin|LCD_CS_Pin|LCD_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  LCD_CS(0);
  #endif
}

void lcd_writeBus(uint8_t data)
{
    HAL_SPI_Transmit(&hspi1, &data, 1, 1);
}


void lcd_writeCMD(uint8_t cmd)
{
    #if SOFTWARE_SPI 

    uint8_t i = 0;

    #ifdef CS_BIT
    LCD_CS(0);
    #endif

    LCD_DC(0);
    LCD_SCK(0);
    for(i = 0; i < 8; i++)
    {
    LCD_MOSI((cmd & (0x80 >> i)));
    LCD_SCK(1);
    LCD_SCK(0);
    }
    #ifdef CS_BIT
    LCD_CS(1);
    #endif

    #elif !SOFTWARE_SPI
    LCD_DC(0);
    lcd_writeBus(cmd);
    LCD_DC(1);
    #endif
}

void lcd_writeData(uint8_t data)
{
  #if SOFTWARE_SPI 
uint8_t i = 0;
    #ifdef CS_BIT
	LCD_CS(0);
    #endif
    LCD_DC(1);
	LCD_SCK(0);
	for(i = 0; i < 8; i++)
	{
		LCD_MOSI((data & (0x80 >> i)));
        LCD_SCK(1);
        LCD_SCK(0);
	}
    #ifdef CS_BIT
        LCD_CS(1);
    #endif
  #elif !SOFTWARE_SPI
    lcd_writeBus(data);
  #endif
}

void lcd_writeData16(uint16_t data)
{
  #if SOFTWARE_SPI 
	uint8_t i = 0;
	
	for(i = 0; i < 2; i++)
	{
		if(i == 0)
		{
			lcd_writeData(data >> 8);
		}
		else
		{
			lcd_writeData(data & 0xFF);
		}
	}
  #elif !SOFTWARE_SPI
    uint8_t tempArr[2];
    tempArr[0] = data >>8;
    tempArr[1] = data & 0xFF;
    HAL_SPI_Transmit(&hspi1, tempArr, 2, 1);
#endif
}



void lcd_setAddrss(uint16_t x, uint16_t y,uint16_t x_e,uint16_t y_e)
{
    lcd_writeCMD(0x2A);
    lcd_writeData(x >> 8);
    lcd_writeData(x & 0xFF);
    lcd_writeData(x_e >> 8);
    lcd_writeData(x_e & 0xFF);
    lcd_writeCMD(0x2B);
    lcd_writeData(y >> 8);
    lcd_writeData(y & 0xFF);
    lcd_writeData(y_e >> 8);
    lcd_writeData(y_e & 0xFF);
    lcd_writeCMD(0x2C);
}

void lcd_clearScreen(uint16_t color)
{
    uint16_t i = 0, j = 0;
    lcd_setAddrss(0, 0 + LCD_Y_SCREEN_OFFSET, LCD_WIDTH - 1, LCD_HEIGHT - 1 + LCD_Y_SCREEN_OFFSET);
    for(i = 0; i < LCD_HEIGHT; i++)
    {
        for(j = 0; j < LCD_WIDTH; j++)
        {
            lcd_writeData16(color);
        }
    }
}

void lcd_init(void)
{
    //gpio_init
    lcd_gpio_init();
    //RST
    LCD_RST(0);
	HAL_Delay(1);
    LCD_RST(1);
    HAL_Delay(5);
	
    LCD_RST(1);
    //lcd para config
    
    lcd_regConfig();
    
}


void lcd_openLight(void)
{
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
}

void lcd_setLight(uint8_t dc)
{
    if(dc >= 5 && dc <= 100)
    {
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, dc * PWM_PERIOD/100);
    }
}

void lcd_closeLight(void)
{

    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
    
}





void lcd_regConfig(void)
{
	lcd_writeCMD(0x11); 
	HAL_Delay(120);
	lcd_writeCMD(0x36); 
	lcd_writeData(0x00);

	lcd_writeCMD(0x3A);
	lcd_writeData(0x55);

	lcd_writeCMD(0xB2);
	lcd_writeData(0x0C);
	lcd_writeData(0x0C);
	lcd_writeData(0x00);
	lcd_writeData(0x33);
	lcd_writeData(0x33); 

	lcd_writeCMD(0xB7); 
	lcd_writeData(0x35);  

	lcd_writeCMD(0xBB);
	lcd_writeData(0x19);

	lcd_writeCMD(0xC0);
	lcd_writeData(0x2C);

	lcd_writeCMD(0xC2);
	lcd_writeData(0x01);

	lcd_writeCMD(0xC3);
	lcd_writeData(0x12);   

	lcd_writeCMD(0xC4);
	lcd_writeData(0x20);  

	lcd_writeCMD(0xC6); 
	lcd_writeData(0x0F);    

	lcd_writeCMD(0xD0); 
	lcd_writeData(0xA4);
	lcd_writeData(0xA1);

	lcd_writeCMD(0xE0);
	lcd_writeData(0xD0);
	lcd_writeData(0x04);
	lcd_writeData(0x0D);
	lcd_writeData(0x11);
	lcd_writeData(0x13);
	lcd_writeData(0x2B);
	lcd_writeData(0x3F);
	lcd_writeData(0x54);
	lcd_writeData(0x4C);
	lcd_writeData(0x18);
	lcd_writeData(0x0D);
	lcd_writeData(0x0B);
	lcd_writeData(0x1F);
	lcd_writeData(0x23);

	lcd_writeCMD(0xE1);
	lcd_writeData(0xD0);
	lcd_writeData(0x04);
	lcd_writeData(0x0C);
	lcd_writeData(0x11);
	lcd_writeData(0x13);
	lcd_writeData(0x2C);
	lcd_writeData(0x3F);
	lcd_writeData(0x44);
	lcd_writeData(0x51);
	lcd_writeData(0x2F);
	lcd_writeData(0x1F);
	lcd_writeData(0x1F);
	lcd_writeData(0x20);
	lcd_writeData(0x23);

	lcd_writeCMD(0x21); 

	lcd_writeCMD(0x29); 
}
