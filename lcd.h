#ifndef _LCD_H
#define _LCD_H

#define RS_PIN 2
#define RW_PIN 1
#define EN_PIN 0
#define CONTROL_PINS PORTC
#define DATA_BUS PORTB

void LCD_send_command(char);
void LCD_send_data(char *);
void LCD_init(void);

#endif
