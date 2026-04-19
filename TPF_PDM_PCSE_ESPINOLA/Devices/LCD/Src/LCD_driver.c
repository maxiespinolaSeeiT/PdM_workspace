/*
 * LCD_driver.c
 *
 *  Created on: Apr 9, 2026
 *      Author: Usuario
 */
#include "LCD_driver.h"
#include "LCD_port.h"
#include "stm32f4xx_hal.h"


// Bits de control
#define LCD_RS 0x01
#define LCD_RW 0x02
#define LCD_EN 0x04
#define LCD_BL 0x08

static void LCD_SendNibble(uint8_t nibble, uint8_t control);
static void LCD_PulseEnable(uint8_t data);

static void LCD_SendNibble(uint8_t nibble, uint8_t control)
{
    uint8_t data = nibble | control | LCD_BL;

    LCD_Port_Write(data);
    LCD_PulseEnable(data);
}


void LCD_Init(void)
{
	MX_I2C1_Init();

    LCD_Port_Delay(50);

    // Secuencia inicialización modo 4 bits
    LCD_SendNibble(0x30, 0);
    LCD_Port_Delay(5);

    LCD_SendNibble(0x30, 0);
    LCD_Port_Delay(1);

    LCD_SendNibble(0x30, 0);
    LCD_SendNibble(0x20, 0);

    // Configuración
    LCD_SendCommand(0x28); // 4 bits, 2 líneas
    LCD_SendCommand(0x08); // display OFF
    LCD_SendCommand(0x01); // clear
    LCD_Port_Delay(2);
    LCD_SendCommand(0x06); // entry mode
    LCD_SendCommand(0x0C); // display ON
}

void LCD_SendCommand(uint8_t cmd)
{
    LCD_SendNibble(cmd & 0xF0, 0); //Acá envía los MSB , RS=0 porque es comando
    LCD_SendNibble((cmd << 4) & 0xF0, 0); //Hace el corrimiento y envía los LSB , RS=0 porque es comando
}

void LCD_SendData(uint8_t data)
{
    LCD_SendNibble(data & 0xF0, LCD_RS);//Acá envía los MSB , RS=1 porque es dato para imprimir en el LCD
    LCD_SendNibble((data << 4) & 0xF0, LCD_RS);//Acá envía los LSB , RS=1 porque es dato para imprimir en el LCD
}

static void LCD_PulseEnable(uint8_t data) //Envia un pulso en alto por 1ms a RS
{
    LCD_Port_Write(data | LCD_EN);
    LCD_Port_Delay(1);

    LCD_Port_Write(data & ~LCD_EN);
    LCD_Port_Delay(1);
}

void LCD_Clear(void)
{
    LCD_SendCommand(0x01);
    LCD_Port_Delay(2);
}

void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t addr ;
    addr = (row == 0) ? 0x80 : 0xC0; //Fila 0 comienza en 0x80 y fila 1 en 0xC0
    addr += col;
    LCD_SendCommand(addr);
}

void LCD_WriteString(char *str)
{
    while (*str != '\0') //recorro el string hasta el final
    {
        LCD_SendData((uint8_t)(*str));
        str++; //muevo el puntero para recorrer el string
    }
}
