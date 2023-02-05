#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

static void LCD_send_symbol(char symbol)
{
    DATA_BUS = symbol;
    CONTROL_PINS = (1 << RS_PIN) | (1 << EN_PIN);
    _delay_us(40);
    CONTROL_PINS = (1 << RS_PIN) | (0 << EN_PIN);
    _delay_ms(2);
}

void LCD_init(void)
{
    LCD_send_command(0x38);
    LCD_send_command(0x01);
    LCD_send_command(0x02);
    LCD_send_command(0x0C);
}

void LCD_send_command(char command)
{ 
    DATA_BUS = command;
    CONTROL_PINS = (0 << RS_PIN) | (1 << EN_PIN); //Signaling to send commands
    _delay_us(40);
    CONTROL_PINS = (0 << RS_PIN) | (0 << EN_PIN); //Signaling to send commands

    if ( command & 0x01 || command & 0x02 ) // If commands are LCD Clear or return home
       _delay_ms(2);
}

void LCD_send_data(char *str_data)
{
    if ( !str_data )
        return;

    int i = 0;
    char cur_symbol;

    while ( str_data[i] ) {
        cur_symbol = str_data[i];
        LCD_send_symbol(cur_symbol);
        i++;
    }
}
