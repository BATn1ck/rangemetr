#define F_CPU 8000000UL
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"

#define MAX_LEN_LCD_STR 16

typedef struct {
    uint16_t whole:12;
    uint16_t frac:4;
} distance_value;

volatile unsigned int pulse = 0, i = 0;
volatile unsigned char c = 0;

static inline void get_fraction(uint8_t, distance_value *);

ISR (INT1_vect)
{
    if ( i == 1 ) {
        TCCR1B = 0;
        pulse = TCNT1;
        TCNT1 = 0;
        i = 0;
    }

    if ( i == 0 ) {
        TCCR1B |= (1<<CS10);
        i = 1;
    }
}

ISR (INT0_vect)
{
    cli();
    while ( !(PIND & (1<<PD2)) ) { }
}

void send_trig(void)
{
    PORTD |= 1 << PD0;
    _delay_us(10);
    PORTD &= ~(1<<PD0);
}

int main(void)
{
    DDRB = 0xFF;
    DDRC = 0x07;

    sei();

    // configure INT0
    GICR |= (1<<INT0);
    MCUCR |= (1<<ISC00);
    DDRD &= ~(1<<PD2);

    // configure INT1
    GICR |= (1<<INT1);
    MCUCR |= (1<<ISC10);
    
    TCCR1A = 0;
    
    DDRD |= 1;
    distance_value a;

    char send_str[MAX_LEN_LCD_STR+1];

    LCD_init();

    while ( 1 ) {
        send_trig();
        a.whole = pulse / 58;
        get_fraction(pulse % 58, &a);
        snprintf(send_str, MAX_LEN_LCD_STR+1, "LEN=%d.%d CM    ", a.whole, a.frac);
        LCD_send_data(send_str);

        LCD_send_command(0xc0); // set position on 2nd row
        snprintf(send_str, MAX_LEN_LCD_STR+1, "FULL=%d.%d CM   ", a.whole+7, a.frac);
        LCD_send_data(send_str);
        
        LCD_send_command(0x02); // return cursor to home
    }

    return 0;
}

static inline void get_fraction(uint8_t remainder, distance_value *fraction)
{
    int frac = remainder * 10 / 58;
    fraction->frac = frac;
}
