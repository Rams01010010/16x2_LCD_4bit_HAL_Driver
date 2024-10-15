/*
 * lcd.c
 *
 *  Created on: Sep 26, 2024
 *  Author: rams
 *  Description: Source code for interfacing
 *  			 16x2 LCD in 4-bit mode.
 */
#include "lcd.h"



/*
 *
 * @description: Controls the delay of pulse given to the EN pin
 * 				 for lcd write operation. Thereby controlling
 * 				 the delay between data writes.
 *
 * @arguments:	 lcd : Pass lcd object's reference.
 *				 delay : Desired delay in milliseconds.
 *
 * @note:		 Max delay possible is 65 seconds.
 *
 * @returns: 	 none
 *
 */
void lcd_setDelay(LCD_t *lcd, uint16_t delay)
{
	// avoid inappropriate delays.
	if(delay < 1)
	{
		delay = 1;
	}
	lcd->delay = delay;
}



/*
 *
 * @description: Pulses the EN pin of the lcd for writing
 * 				 data.
 *
 * @arguments:	 lcd : lcd object's reference.
 *
 * @returns: 	 none
 *
 */
void lcd_pulseEn(const LCD_t *const lcd)
{
	// provide a rising edge.
	HAL_GPIO_WritePin(lcd->EN_port, lcd->EN_pin, GPIO_PIN_SET);
	HAL_Delay(lcd->delay);
	HAL_GPIO_WritePin(lcd->EN_port, lcd->EN_pin, GPIO_PIN_RESET);
}



/*
 *
 * @description: Writes the instructions in 4-bit mode.
 *
 * @arguments:	 lcd : lcd object's reference.
 *				 cmd : instruction to be written (8 bits).
 *
 * @returns: 	 none
 *
 */
void lcd_writeCommand(const LCD_t *const lcd, uint8_t cmd)
{
	// RS - 0 for selecting instruction register.
	// RW - 0 for write operation.
	HAL_GPIO_WritePin(lcd->RS_port, lcd->RS_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->RW_port, lcd->RW_pin, GPIO_PIN_RESET);

	// write the upper nibble (D7-D4) first (for 4-bit mode).
	HAL_GPIO_WritePin(lcd->D4_port, lcd->D4_pin, (cmd >> 4) & 0x1);
	HAL_GPIO_WritePin(lcd->D5_port, lcd->D5_pin, (cmd >> 5) & 0x1);
	HAL_GPIO_WritePin(lcd->D6_port, lcd->D6_pin, (cmd >> 6) & 0x1);
	HAL_GPIO_WritePin(lcd->D7_port, lcd->D7_pin, (cmd >> 7) & 0x1);
	lcd_pulseEn(lcd);

	// write the lower nibble (D3-D0).
	HAL_GPIO_WritePin(lcd->D4_port, lcd->D4_pin, (cmd >> 0) & 0x1);
	HAL_GPIO_WritePin(lcd->D5_port, lcd->D5_pin, (cmd >> 1) & 0x1);
	HAL_GPIO_WritePin(lcd->D6_port, lcd->D6_pin, (cmd >> 2) & 0x1);
	HAL_GPIO_WritePin(lcd->D7_port, lcd->D7_pin, (cmd >> 3) & 0x1);
	lcd_pulseEn(lcd);
}



/*
 *
 * @description: Writes the data in 4-bit mode.
 *
 * @arguments:	 lcd : lcd object's reference.
 *				 data : data to be written (8 bits).
 *
 * @returns: 	 none
 *
 */
void lcd_writeData(const LCD_t *const lcd, uint8_t data)
{
	// RS - 1 for selecting data register.
	// RW - 0 for write operation.
	HAL_GPIO_WritePin(lcd->RS_port, lcd->RS_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(lcd->RW_port, lcd->RW_pin, GPIO_PIN_RESET);

	// write the upper nibble (D7-D4) first (for 4-bit mode).
	HAL_GPIO_WritePin(lcd->D4_port, lcd->D4_pin, (data >> 4) & 0x1);
	HAL_GPIO_WritePin(lcd->D5_port, lcd->D5_pin, (data >> 5) & 0x1);
	HAL_GPIO_WritePin(lcd->D6_port, lcd->D6_pin, (data >> 6) & 0x1);
	HAL_GPIO_WritePin(lcd->D7_port, lcd->D7_pin, (data >> 7) & 0x1);
	lcd_pulseEn(lcd);

	// write the lower nibble (D3-D0).
	HAL_GPIO_WritePin(lcd->D4_port, lcd->D4_pin, (data >> 0) & 0x1);
	HAL_GPIO_WritePin(lcd->D5_port, lcd->D5_pin, (data >> 1) & 0x1);
	HAL_GPIO_WritePin(lcd->D6_port, lcd->D6_pin, (data >> 2) & 0x1);
	HAL_GPIO_WritePin(lcd->D7_port, lcd->D7_pin, (data >> 3) & 0x1);
	lcd_pulseEn(lcd);
}



/*
 *
 * @description: Clears the display.
 *
 * @arguments:	 lcd : Pass lcd object's reference.
 *
 * @note:		 This also sets the entry mode to increment.
 *
 * @returns: 	 none
 *
 */
void lcd_clearDisplay(const LCD_t *const lcd)
{
	lcd_writeCommand(lcd, LCD_CMD_CLEAR_DISPLAY);
}



/*
 *
 * @description: Sets the cursor to (0,0) and moves
 * 				 the display appropriately.
 *
 * @arguments:	 lcd : Pass lcd object's reference.
 *
 * @note:		 This also sets the entry mode to increment.
 *
 * @returns: 	 none
 *
 */
void lcd_returnHome(const LCD_t *const lcd)
{
	lcd_writeCommand(lcd, LCD_CMD_RETURN_HOME);
}


/*
 *
 * @description: Sets entry mode. This is run every time a
 * 				 read or write operation is performed and so
 * 				 this controls how characters will be
 * 				 written consecutively.
 *
 * @arguments:	 lcd : Pass lcd object's reference.
 * 				 increment: 1 - increment, 0 - decrement.
 * 				 display_shift: 1 - display-shift, 0 - cursor-shift.
 *
 * @returns: 	 none
 *
 */
void lcd_setEntryMode(const LCD_t *const lcd, uint8_t increment, uint8_t display_shift)
{
	// set i/d bit to increment
	// set s bit to display_shift
	lcd_writeCommand(lcd, LCD_CMD_ENTRY_MODE | (increment << 1) | display_shift);
}



/*
 *
 * @description: Sets display on or off.
 *
 * @arguments:	 lcd : Pass lcd object's reference.
 * 				 display_on: 1 - ON, 0 - OFF.
 *
 * @note:		 use LCD_DISPLAY_ON or LCD_DISPLAY_OFF
 * 				 for better readability.
 *
 * @returns: 	 none
 *
 */
void lcd_displayCtrl(LCD_t *const lcd, uint8_t display_on)
{
	// clear display bit & set according to display_on
	lcd->displayCtrl = (lcd->displayCtrl & ~(0x4)) | (display_on << 2);
	lcd_writeCommand(lcd,lcd->displayCtrl);
}



/*
 *
 * @description: Shows or hides the cursor.
 *
 * @arguments:	 lcd : Pass lcd object's reference.
 * 				 show_cursor: 1 - ON, 0 - OFF.
 *
 * @note:		 use LCD_SHOW_CURSOR or LCD_HIDE_CURSOR
 * 				 for better readability.
 *
 * @returns: 	 none
 *
 */
void lcd_showCursor(LCD_t *const lcd, uint8_t show_cursor)
{
	// clear cursor bit & set according to show_cursor
	lcd->displayCtrl = (lcd->displayCtrl & ~(0x2)) | (show_cursor << 1);
	lcd_writeCommand(lcd,lcd->displayCtrl);
}



/*
 *
 * @description: Set cursor blink on or off.
 *
 * @arguments:	 lcd : Pass lcd object's reference.
 * 				 blink_on: 1 - ON, 0 - OFF.
 *
 * @note:		 use LCD_BLINK_ON or LCD_BLINK_OFF
 * 				 for better readability.
 *
 * @returns: 	 none
 *
 */
void lcd_cursorBlink(LCD_t *const lcd, uint8_t blink_on)
{
	// clear blink bit & set according to blink_on
	lcd->displayCtrl = (lcd->displayCtrl & ~(0x1)) | blink_on;
	lcd_writeCommand(lcd,lcd->displayCtrl);
}



/*
 *
 * @description: Shifts only the cursor right/left.
 *
 * @arguments:	 lcd : Pass lcd object's reference.
 * 				 direction: 1 - Right, 0 - Left.
 *
 * @note:		 use LCD_SHIFT_RIGHT or LCD_SHIFT_LEFT
 * 				 for better readability.
 *
 * @returns: 	 none
 *
 */
void lcd_cursorShift(const LCD_t *const lcd, uint8_t direction)
{
	// set S/C - C (0x0)
	// set R/L - (direction << 3)
	lcd_writeCommand(lcd, (LCD_CMD_DISPLAY_SHIFT & ~(0x8)) | (direction << 2));
}



/*
 *
 * @description: Shifts the whole display right/left.
 *
 * @arguments:	 lcd : Pass lcd object's reference.
 * 				 direction: 1 - Right, 0 - Left.
 *
 * @note:		 use LCD_SHIFT_RIGHT or LCD_SHIFT_LEFT
 * 				 for better readability.
 *
 * @returns: 	 none
 *
 */
void lcd_displayShift(const LCD_t *const lcd, uint8_t direction)
{
	// set S/C - S (0x8)
	// set R/L - (direction << 3)
	lcd_writeCommand(lcd, LCD_CMD_DISPLAY_SHIFT | 0x8 | (direction << 2));
}



/*
 *
 * @description: Locates the cursor in row,col position
 *
 * @arguments:	 lcd : Pass lcd object's reference.
 * 				 row: 0 - first line, 1 - second line.
 * 				 col: 0-39.
 *
 * @note:		 This also sets the entry mode to increment.
 *
 * @returns: 	 none
 *
 */
void lcd_setCursor(const LCD_t *const lcd, uint8_t row, uint8_t col)
{
	lcd_writeCommand(lcd, LCD_CMD_SET_DDRAM_ADR | (row<<6) | col);
}



/*
 * @description: Programs custom patterns in CGRAM at
 * 				 appropriate address location based on pattern
 * 				 id.
 *
 * @arguments:	 lcd : Pass lcd object's reference.
 *				 pattern_id : The pattern's id (see note).
 *				 *pattern : 8 rows of the pattern each of 5-bits.
 *
 * @note:		 Valid values for pattern_id: 0,1,2,3,4,5,6,7.
 * 				 Because maximum custom characters that can be
 * 				 programmed is 8.
 *
 * 				 Since 5x8 is the font being used, array of
 * 				 8 5-bit integers is to be passed.
 * 				 eg: uint8_t p[8] = {0b00100, 0b01110, 0b00100, 0b01110, 0b00100, 0b01110, 0b00100, 0b01110};
 *
 * 				 Call this function after initialization as this
 * 				 function will clear the display.
 *
 * @returns: 	 none
 *
 */
void lcd_setPattern(const LCD_t *const lcd, uint8_t pattern_id, uint8_t *pattern)
{
	// save current delay in a variable and fasten the process.
	uint8_t delay = lcd->delay;
	lcd_setDelay((LCD_t*)lcd, 1);

	// avoid inappropriate pattern ids.
	if(pattern_id < 0 || pattern_id > 7)
	{
		pattern_id = 0;
	}

	// write the CGRAM address (0b01AAA000);
	// where 'A' is the patternId.
	lcd_writeCommand(lcd, LCD_CMD_SET_CGRAM_ADR | (pattern_id << 3));

	// write each row of the pattern in data register.
	int i = 0;
	while(i < 8)
	{
		lcd_writeData(lcd, *(pattern+i));
		i++;
	}

	lcd_clearDisplay(lcd);
	//set the delay set by the user.
	lcd_setDelay((LCD_t*)lcd, delay);
}



/*
 * @description: Prints the given message in lcd display.
 *
 * @arguments:	 lcd : Pass lcd object's reference.
 *				 msg : Message string.
 *
 * @returns: 	 none
 *
 */
void lcd_printMsg(const LCD_t *const lcd, char *msg)
{
	while(*msg != '\0')
	{
		lcd_writeData(lcd, *(uint8_t*)msg);
		msg++;
	}
}



/*
 * @description: Prints the given pattern/character in lcd display.
 *
 * @arguments:	 lcd : Pass lcd object's reference.
 *				 pattern_id : Accepted values -> pattern_id or character (ASCII).
 *
 * @note:		 Patterns can be printed by passing pattern_id
 * 				 Single characters can be printed by passing
 * 				 characters in uint8_t (ASCII).
 *
 * @returns: 	 none
 *
 */
void lcd_printChar(const LCD_t *const lcd, uint8_t pattern_id)
{
	lcd_writeData(lcd, pattern_id);
}



/*
 * @description: Resets the LCD and sets 4-bit mode.
 *
 * @arguments:	 lcd : Pass lcd object's reference.
 *
 * @note:		 Make sure to initialize before printing.
 * 				 Or call 'lcdInit' for default initialization.
 *
 * @returns: 	 none
 *
 */
void lcd_reset(LCD_t *const lcd)
{
	lcd_setDelay(lcd, 0);
	lcd->displayCtrl = LCD_CMD_DISPLAY_CTRL;
	// reset lcd by instruction. write 0b0011 thrice
	// and set to 4-bit mode. since writing 0x3 sets
	// lcd in 8-bit mode.
	lcd_writeCommand(lcd, 0x33);
	lcd_writeCommand(lcd, 0x32);

	// function set to 4-bit mode with 2 lines & 5x8 dots
	lcd_writeCommand(lcd, 0x28);
}



/*
 * @description: Initializes the LCD with cursor-on & blink-off
 * 				 and to move right after every character written.
 *
 * @arguments:	 lcd : Pass lcd object's reference.
 *
 * @returns: 	 none
 *
 */
void lcd_init(const LCD_t *const lcd)
{
	lcd_reset((LCD_t*)lcd);

	lcd_displayCtrl((LCD_t*)lcd, LCD_DISPLAY_ON);
	lcd_showCursor((LCD_t*)lcd, LCD_SHOW_CURSOR);

	lcd_clearDisplay(lcd);

	lcd_setEntryMode(lcd, 1, 0);
}
