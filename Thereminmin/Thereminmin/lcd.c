#define LCD_E 	3
#define LCD_RS	2
#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <xc.h>

void LCD_strobe_lcd_e(void) {
	PORTC |= (1<<LCD_E);	// E high
	_delay_ms(1);			// nodig
	PORTC &= ~(1<<LCD_E);  	// E low
	_delay_ms(1);			// nodig?
}




void LCD_write_command(unsigned char byte)
{
	// First nibble.
	PORTC = byte;
	PORTC &= ~(1<<LCD_RS);
	LCD_strobe_lcd_e();

	// Second nibble
	PORTC = (byte<<4);
	PORTC &= ~(1<<LCD_RS);
	LCD_strobe_lcd_e();
}

void LCD_clear() {
	LCD_write_command (0x01);						//Leeg display
	_delay_ms(2);
	LCD_write_command (0x80);						//Cursor terug naar start
}

void LCD_init() {
	// PORTC output mode and all low (also E and RS pin)
	DDRC = 0xFF;
	PORTC = 0x00;

	// Step 2 (table 12)
	PORTC = 0x20;	// function set
	LCD_strobe_lcd_e();

	// Step 3 (table 12)
	PORTC = 0x20;   // function set
	LCD_strobe_lcd_e();
	PORTC = 0x80;
	LCD_strobe_lcd_e();

	// Step 4 (table 12)
	PORTC = 0x00;   // Display on/off control
	LCD_strobe_lcd_e();
	PORTC = 0xF0;
	LCD_strobe_lcd_e();

	// Step 4 (table 12)
	PORTC = 0x00;   // Entry mode set
	LCD_strobe_lcd_e();
	PORTC = 0x60;
	LCD_strobe_lcd_e();
	
	LCD_write_command(0x01);
	
	LCD_clear();
}

void LCD_write_data(unsigned char byte) {
	// First nibble.
	PORTC = byte;
	PORTC |= (1<<LCD_RS);
	LCD_strobe_lcd_e();

	// Second nibble
	PORTC = (byte<<4);
	PORTC |= (1<<LCD_RS);
	LCD_strobe_lcd_e();
}

void LCD_display_text(char *str) {
	while(*str) {
		LCD_write_data(*str++);
	}
}

void LCD_set_cursor(int position) {
	unsigned char cursor = (position | (1<<7));
	LCD_write_command(cursor);
}