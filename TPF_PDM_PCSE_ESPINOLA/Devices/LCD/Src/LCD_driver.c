/*
 * LCD_driver.c
 *
 *  Created on: Apr 9, 2026
 *      Author: Maximiliano Ariel Espínola
 */
#include "LCD_driver.h"
#include "LCD_port.h"

// Bits de control del expansor PCF8574
#define LCD_RS 0x01
#define LCD_RW 0x02
#define LCD_EN 0x04
#define LCD_BL 0x08

// Comandos de inicialización
#define LCD_CMD_8BIT_MODE     0x30
#define LCD_CMD_4BIT_MODE     0x20
#define LCD_CMD_4BIT_2LINE    0x28  // 4 bits, 2 líneas
#define LCD_CMD_DISPLAY_OFF   0x08  // Display OFF
#define LCD_CMD_CLEAR         0x01  // Clear display
#define LCD_CMD_ENTRY_MODE    0x06  // Entry mode: incrementar, no shift
#define LCD_CMD_DISPLAY_ON    0x0C  // Display ON, cursor OFF, blink OFF

// Direcciones de filas del LCD
#define LCD_ROW0_ADDR         0x80  // Dirección inicio fila 0
#define LCD_ROW1_ADDR         0xC0  // Dirección inicio fila 1

// Delays de inicialización (ms)
#define LCD_DELAY_POWER_ON    50
#define LCD_DELAY_INIT_1      5
#define LCD_DELAY_INIT_2      1
#define LCD_DELAY_CLEAR       2
#define LCD_DELAY_PULSE       1

//Mascara para protocolo de 4 bits 11110000
#define LCD_NIBBLE_HIGH_MASK  0xF0
#define BITS_SHITF 4
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
    LCD_Port_Delay(LCD_DELAY_POWER_ON);

    // Secuencia inicialización modo 4 bits
    LCD_SendNibble(LCD_CMD_8BIT_MODE, 0);
    LCD_Port_Delay(LCD_DELAY_INIT_1);

    LCD_SendNibble(LCD_CMD_8BIT_MODE, 0);
    LCD_Port_Delay(LCD_DELAY_INIT_2);

    LCD_SendNibble(LCD_CMD_8BIT_MODE, 0);
    LCD_SendNibble(LCD_CMD_4BIT_MODE, 0);

    // Configuración
    LCD_SendCommand(LCD_CMD_4BIT_2LINE);
    LCD_SendCommand(LCD_CMD_DISPLAY_OFF);
    LCD_SendCommand(LCD_CMD_CLEAR);
    LCD_Port_Delay(LCD_DELAY_CLEAR);
    LCD_SendCommand(LCD_CMD_ENTRY_MODE);
    LCD_SendCommand(LCD_CMD_DISPLAY_ON);
}

void LCD_SendCommand(uint8_t cmd)
{
    LCD_SendNibble(cmd & LCD_NIBBLE_HIGH_MASK, 0);
    LCD_SendNibble((cmd << BITS_SHITF) & LCD_NIBBLE_HIGH_MASK, 0);
}

void LCD_SendData(uint8_t data)
{
    LCD_SendNibble(data & LCD_NIBBLE_HIGH_MASK, LCD_RS);
    LCD_SendNibble((data << BITS_SHITF) & LCD_NIBBLE_HIGH_MASK, LCD_RS);
}

static void LCD_PulseEnable(uint8_t data)
{
    LCD_Port_Write(data | LCD_EN);
    LCD_Port_Delay(LCD_DELAY_PULSE);

    LCD_Port_Write(data & ~LCD_EN);
    LCD_Port_Delay(LCD_DELAY_PULSE);
}

void LCD_Clear(void)
{
    LCD_SendCommand(LCD_CMD_CLEAR);
    LCD_Port_Delay(LCD_DELAY_CLEAR);
}

void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t addr;
    addr = (row == 0) ? LCD_ROW0_ADDR : LCD_ROW1_ADDR;
    addr += col;
    LCD_SendCommand(addr);
}

void LCD_WriteString(char *str)
{
    while (*str != '\0')
    {
        LCD_SendData((uint8_t)(*str));
        str++;
    }
}
