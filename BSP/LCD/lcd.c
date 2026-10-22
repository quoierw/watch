#include "main.h"
#include "lcd_init.h"
#include "lcd.h"
#include "lcdfont.h"




void lcd_fill(uint16_t x, uint16_t y, uint16_t x_e, uint16_t y_e, uint16_t color)
{
    uint16_t i, j;
    lcd_setAddrss(x, y + LCD_Y_SCREEN_OFFSET, x_e - 1, y_e + LCD_Y_SCREEN_OFFSET - 1);
    for(i = y; i < y_e; i++)
    {
        for(j = x; j < x_e; j ++)
        {
            lcd_writeData16(color);
        }
    }
}

void lcd_color_fill(uint16_t x, uint16_t y, uint16_t x_e, uint16_t y_e, uint16_t* color)
{
    uint16_t width = x_e - x - 1;
    uint16_t height = y_e - y - 1;
    uint32_t numOfpixel = width * height;
    uint32_t byteOfWrite = numOfpixel * 2u;

    lcd_setAddrss(x, y + LCD_Y_SCREEN_OFFSET, x_e - 1, y_e + LCD_Y_SCREEN_OFFSET - 1);
    //hal_spi_dma;

}

void lcd_draw_point(uint16_t x, uint16_t y,uint16_t color)
{
     lcd_setAddrss(x, y + LCD_Y_SCREEN_OFFSET, x, y + LCD_Y_SCREEN_OFFSET);
    //lcd_setAddrss(x, y, x, y);
    lcd_writeData16(color);
}

void lcd_draw_line(uint16_t x, uint16_t y, uint16_t x_e, uint16_t y_e, uint16_t color)
{
    uint16_t i = 0;
    int16_t x_c, y_c, ldtx, ldty, ldistance, incx, incy;
    int16_t lerrx = 0, lerry = 0;

    x_c = x;
    y_c = y;
    ldtx = x_e - x;
    ldty = y_e - y;

    if(ldtx > 0) incx = 1;
    else if(ldtx == 0) incx = 0;
    else {incx = -1; ldtx = - ldtx;}

    if(ldty > 0) incy = 1;
    else if(ldty == 0) incy = 0;
    else {incy = -1; ldty = - ldty;}

    if(ldtx > ldty) ldistance = ldtx;
    else ldistance = ldty;

    for(i = 0; i < ldistance + 1; i++)
    {
        lcd_draw_point(x_c, y_c, color);
        lerrx += ldtx;
        lerry += ldty;
        if(lerrx > ldistance)
        {
            lerrx -= ldistance;
            x_c += incx;
        }
        if(lerry > ldistance)
        {
            lerry -= ldistance;
            y_c += incy;
        }
        
    }
    
}

void lcd_draw_rectangle(uint16_t x, uint16_t y, uint16_t x_e, uint16_t y_e, uint16_t color)
{
    lcd_draw_line(x, y, x_e, y, color);
    lcd_draw_line(x, y, x, y_e, color);
    lcd_draw_line(x_e, y_e, x_e, y, color);
    lcd_draw_line(x_e, y_e, x, y_e, color);

}

void lcd_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color)
{
    int a = 0, b = r;
    while(a <= b)
    {
        lcd_draw_point(x+a, y+b, color);
        lcd_draw_point(x+a, y-b, color);
        lcd_draw_point(x-a, y+b, color);
        lcd_draw_point(x-a, y-b, color);
        lcd_draw_point(x+b, y+a, color);
        lcd_draw_point(x-b, y+a, color);
        lcd_draw_point(x+b, y-a, color);
        lcd_draw_point(x-b, y-a, color);
        a++;
        if((a*a)+(b*b) > (r*r))
        {
            b--;
        }
    }
}




void lcd_show_char(uint16_t x, uint16_t y, uint8_t showChar, uint16_t bc, uint16_t fc, uint8_t sizey, uint8_t mode)
{
    uint16_t i = 0, j = 0;
    uint16_t x0 = x;
    uint8_t m = 0;
    uint8_t sizex = sizey / 2;
    uint8_t byteOfChar = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * sizey;
    uint8_t tempVal;
    showChar = showChar - ' ';
    lcd_setAddrss(x, y + LCD_Y_SCREEN_OFFSET, x + sizex - 1, y + sizey - 1 + LCD_Y_SCREEN_OFFSET);

    for(i = 0; i < byteOfChar; i++)
    {
        if(sizey == 12) tempVal = ascii_1206[showChar][i];
        else if(sizey == 16) tempVal = ascii_1608[showChar][i];
        else if(sizey == 24) tempVal = ascii_2412[showChar][i];
        else if(sizey == 32) tempVal = ascii_3216[showChar][i];
        else return;

        for(j = 0; j < 8; j++)
        {
            if(!mode)
            {
                if(tempVal & (0x01 << j))
                    lcd_writeData16(fc);
                else
                    lcd_writeData16(bc);
                m++;
                if(m%sizex == 0)
                {
                    m = 0; 
                    break;
                }
            }
            else
            {
                if(tempVal & (0x01 << j))
                    lcd_draw_point(x, y, fc);
                x++;
                if(x- x0 == sizex)
                {
                    y++;
                    x = x0;
                    break;
                }
            }
        }

    }

}

void lcd_show_string(uint16_t x, uint16_t y, const uint8_t *str, uint16_t bc, uint16_t fc, uint8_t sizey, uint8_t mode)
{
    uint8_t i = 0;
    while(*str != '\0')
    {
        if((x + sizey / 2) > LCD_WIDTH)
        {
            x = 0;
            y += sizey;
        }
        lcd_show_char(x, y, *str, bc, fc, sizey, mode);
        str++;
        x += sizey / 2;
    }
}

void lcd_show_chinese12x12(uint16_t x, uint16_t y, uint8_t * showChar, uint16_t bc, uint16_t fc, uint8_t sizey, uint8_t mode)
{
    uint16_t i = 0, j = 0, k = 0;
    uint16_t x0 = x;
    uint16_t m = 0;
    uint16_t byteOfChar = (sizey / 8 + ((sizey %8) ? 1 : 0)) * sizey;
    uint8_t numOfChinese = sizeof(tfont12) / sizeof(typFNT_GB12);

    for(i = 0; i < numOfChinese; i++)
    {
        if((showChar[0] == tfont12[i].Index[0]) && (showChar[1] == tfont12[i].Index[1]))
        {
            lcd_setAddrss(x, y + LCD_Y_SCREEN_OFFSET, x + sizey - 1, y + LCD_Y_SCREEN_OFFSET + sizey - 1);
            for(j = 0; j < byteOfChar; j++)
            {
                for(k = 0; k < 8; k++)
                {
                    if(!mode)
                    {
                        if(tfont12[i].Msk[j] & (0x01 << k))
                            lcd_writeData16(fc);
                        else
                            lcd_writeData16(bc);
                        m++;
                        if(m%sizey == 0)
                        {
                            m = 0;
                            break;
                        }
                    }
                    else
                    {
                        if(tfont12[i].Msk[j] & (0x01 << k))
                            lcd_draw_point(x, y, fc);
                        x++;
                        if(x- x0 == sizey)
                        {
                            y++;
                            x = x0;
                            break;
                        }
                    }
                }

            }
            break;
        }
    }
}

void lcd_show_chinese16x16(uint16_t x, uint16_t y, uint8_t * showChar, uint16_t bc, uint16_t fc, uint8_t sizey, uint8_t mode)
{
    uint16_t i = 0, j = 0, k = 0;
    uint16_t x0 = x;
    uint16_t byteOfChar = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    uint8_t numOfchinese = sizeof(tfont16) / sizeof(typFNT_GB16);
    uint8_t m = 0;

    for(i = 0; i < numOfchinese; i++)
    {
        if((showChar[0] == tfont16[i].Index[0]) && (showChar[1] == tfont16[i].Index[1]))
        {
            lcd_setAddrss(x, y + LCD_Y_SCREEN_OFFSET, x + sizey - 1, y + sizey - 1 + LCD_Y_SCREEN_OFFSET);
            for(j = 0; j < byteOfChar; j++)
            {
                for(k = 0; k < 8; k++)
                {
                    if(!mode)
                    {
                        if(tfont16[i].Msk[j] & (0x01 << k))
                            lcd_writeData16(fc);
                        else
                            lcd_writeData16(bc);
                        m++;
                        if(m%sizey == 0)
                        {
                            m = 0;
                            break;
                        }
                    }
                    else
                    {
                        if(tfont16[i].Msk[j] & (0x01 << k))
                            lcd_draw_point(x, y, fc);
                        x++;
                        if(x - x0 == sizey)
                        {
                            y++;
                            x = x0;
                            break;
                        }
                    }
                }
            }

            break;
        }

    }
}

void lcd_show_chinese24x24(uint16_t x, uint16_t y, uint8_t * showChar, uint16_t bc, uint16_t fc, uint8_t sizey, uint8_t mode)
{
    uint16_t i = 0, j = 0, k = 0;
    uint16_t x0 = x;
    uint16_t byteOfChar = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    uint8_t numOfchinese = sizeof(tfont24) / sizeof(typFNT_GB24);
    uint8_t m = 0;

    for(i = 0; i < numOfchinese; i++)
    {
        if((showChar[0] == tfont24[i].Index[0]) && (showChar[1] == tfont24[i].Index[1]))
        {
            lcd_setAddrss(x, y + LCD_Y_SCREEN_OFFSET, x + sizey - 1, y + sizey - 1 + LCD_Y_SCREEN_OFFSET);
            for(j = 0; j < byteOfChar; j++)
            {
                for(k = 0; k < 8; k++)
                {
                    if(!mode)
                    {
                        if(tfont24[i].Msk[j] & (0x01 << k))
                            lcd_writeData16(fc);
                        else
                            lcd_writeData16(bc);
                        m++;
                        if(m%sizey == 0)
                        {
                            m = 0;
                            break;
                        }
                    }
                    else
                    {
                        if(tfont24[i].Msk[j] & (0x01 << k))
                            lcd_draw_point(x, y, fc);
                        x++;
                        if(x - x0 == sizey)
                        {
                            y++;
                            x = x0;
                            break;
                        }
                    }
                }
            }

            break;
        }

    }
}

void lcd_show_chinese32x32(uint16_t x, uint16_t y, uint8_t * showChar, uint16_t bc, uint16_t fc, uint8_t sizey, uint8_t mode)
{
    uint16_t i = 0, j = 0, k = 0;
    uint16_t x0 = x;
    uint16_t byteOfChar = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    uint8_t numOfchinese = sizeof(tfont32) / sizeof(typFNT_GB24);
    uint8_t m = 0;

    for(i = 0; i < numOfchinese; i++)
    {
        if((showChar[0] == tfont32[i].Index[0]) && (showChar[1] == tfont32[i].Index[1]))
        {
            lcd_setAddrss(x, y + LCD_Y_SCREEN_OFFSET, x + sizey - 1, y + sizey - 1 + LCD_Y_SCREEN_OFFSET);
            for(j = 0; j < byteOfChar; j++)
            {
                for(k = 0; k < 8; k++)
                {
                    if(!mode)
                    {
                        if(tfont32[i].Msk[j] & (0x01 << k))
                            lcd_writeData16(fc);
                        else
                            lcd_writeData16(bc);
                        m++;
                        if(m%sizey == 0)
                        {
                            m = 0;
                            break;
                        }
                    }
                    else
                    {
                        if(tfont32[i].Msk[j] & (0x01 << k))
                            lcd_draw_point(x, y, fc);
                        x++;
                        if(x - x0 == sizey)
                        {
                            y++;
                            x = x0;
                            break;
                        }
                    }
                }
            }

            break;
        }

    }
}

void lcd_show_chinese(uint16_t x, uint16_t y, uint8_t * showChar, uint16_t bc, uint16_t fc, uint8_t sizey, uint8_t mode)
{
    while(*showChar != '\0')
    {
        if (sizey == 12)
            lcd_show_chinese12x12(x, y, showChar, bc, fc, sizey, mode);
        else if(sizey == 16)
             lcd_show_chinese16x16(x, y, showChar, bc, fc, sizey, mode);
        else if(sizey == 24)
             lcd_show_chinese16x16(x, y, showChar, bc, fc, sizey, mode);
        else if(sizey == 32)
             lcd_show_chinese16x16(x, y, showChar, bc, fc, sizey, mode);
        else
            return;
        showChar += 2;
        x += sizey;
        
   }
}

uint32_t mypow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while(n--)result *= m;
    return result;
}

void lcd_show_intNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t bc, uint16_t fc, uint8_t sizey)
{
    uint8_t i = 0;
    uint8_t temp = 0;
    uint8_t enshow = 0;
    uint8_t sizex = sizey / 2;
    for(i = 0; i < len; i++)
    {
        temp = (num/mypow(10, len - i - 1))%10;
        if(enshow == 0 && i < (len - 1))
        {
            if(temp == 0)
            {
                lcd_show_char(x, y, ' ', bc, fc, sizey, 0);
                continue;
            }
            enshow = 1;
        }
        lcd_show_char(x + i *sizex, y, temp + 48, bc, fc, sizey, 0);
    }
}

void lcd_show_floatNum(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t bc, uint16_t fc, uint8_t sizey)
{
    uint32_t num1 = num * 100;
    uint32_t integer = num;
    uint32_t fraction = num1  %100;
    lcd_show_intNum(x, y, integer, len, bc, fc, sizey);
    lcd_show_char(x + len * (sizey / 2), y, '.', bc, fc, sizey, 0);
    lcd_show_intNum(x + (len + 1) * (sizey / 2), y, fraction, 2, bc, fc, sizey);
}

static void test(void)
{
// lcd_init();
// lcd_clearScreen(WHITE);

// lcd_fill(0, 0, LCD_WIDTH /2 , LCD_HEIGHT / 2, BLACK);

// lcd_fill(LCD_WIDTH /2, 0, LCD_WIDTH, LCD_HEIGHT / 2, RED);
// 这里有弧度，现在是可以看到的。
// lcd_draw_point(12,12,WHITE);
// lcd_draw_point(0,34,WHITE);
// lcd_draw_point(119,0,WHITE);
// lcd_draw_point(120,1,WHITE);
// lcd_draw_point(119,1,GREEN);
// lcd_draw_point(120,0,GREEN);	

// lcd_draw_line(0, 150, 239, 169, BLACK);
// lcd_draw_line(0, 169, 239, 150, BLACK);

// lcd_draw_line(150, 0, 169, 239, BLACK);
// lcd_draw_line(169, 0, 150, 239, BLACK);

// lcd_draw_rectangle(60, 200, 100, 240, BLACK);


// lcd_draw_circle(119, 139, 119, GREEN);
// lcd_show_char(0, 52, 'a', WHITE, BLACK, 12, 0);
// lcd_show_char(60, 52, 'b', WHITE, BLACK, 16, 0);
// lcd_show_char(80, 52, 'c', WHITE, BLACK, 24, 0);
// lcd_show_char(100, 52, 'd', WHITE, BLACK, 32, 0);

// lcd_show_char(0, 52, 'a', WHITE, BLACK, 12, 0);
// lcd_show_char(60, 52, 'b', WHITE, BLACK, 16, 0);
// lcd_show_char(80, 52, 'c', WHITE, BLACK, 24, 0);
// lcd_show_char(100, 52, 'd', WHITE, BLACK, 32, 0);

// lcd_show_char(0, 100, 'a', WHITE, BLACK, 12, 1);
// lcd_show_char(60, 100, 'b', WHITE, BLACK, 16, 1);
// lcd_show_char(80, 100, 'c', WHITE, BLACK, 24, 1);
// lcd_show_char(100, 100, 'd', WHITE, BLACK, 32, 1);
// lcd_show_string(100, 200, "hello world", WHITE, BLACK, 32, 0);
// lcd_show_chinese12x12(50 ,50 , zhongArr, WHITE, BLACK, 12, 0);
// lcd_show_chinese12x12(60 ,50 , zhongArr, WHITE, BLACK, 12, 1);
// lcd_show_chinese16x16(50, 80, zhongArr, WHITE, BLACK, 16, 0);
// lcd_show_chinese16x16(70, 80, zhongArr, WHITE, BLACK, 16, 1);
// lcd_show_chinese24x24(50, 120, zhongArr, WHITE, BLACK, 24, 0);
// lcd_show_chinese24x24(70, 120, zhongArr, WHITE, BLACK, 24, 1);
// lcd_show_chinese32x32(50, 150, zhongArr, WHITE, BLACK, 32, 0);
// lcd_show_chinese32x32(80, 150, zhongArr, WHITE, BLACK, 32, 1);


// lcd_show_intNum(100, 0, 0, 4, WHITE, BLACK, 32);

// lcd_show_floatNum(40, 0, 0.56, 1, WHITE, BLACK, 32);
}