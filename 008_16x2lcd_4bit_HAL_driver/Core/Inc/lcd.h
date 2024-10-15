/*
 * lcd.h
 *
 *  Created on: Sep 26, 2024
 *      Author: rams
 */
#include "main.h"

#ifndef INC_LCD_H_
#define INC_LCD_H_

// Configuration Bits
#define LCD_CMD_CLEAR_DISPLAY   0b00000001
#define LCD_CMD_RETURN_HOME     0b00000010
#define LCD_CMD_ENTRY_MODE      0b00000100
#define LCD_CMD_DISPLAY_CTRL    0b00001000
#define LCD_CMD_DISPLAY_SHIFT   0b00010000
#define LCD_CMD_FUNCTION_SET    0b00100000
#define LCD_CMD_SET_CGRAM_ADR   0b01000000
#define LCD_CMD_SET_DDRAM_ADR   0b10000000

#define LCD_DISPLAY_ON   1
#define LCD_DISPLAY_OFF  0
#define LCD_SHOW_CURSOR  1
#define LCD_HIDE_CURSOR  0
#define LCD_BLINK_ON	 1
#define LCD_BLINK_OFF 	 0
#define LCD_SHIFT_RIGHT  1
#define LCD_SHIFT_LEFT   0

typedef struct
{
	// Hardware initializations
	GPIO_TypeDef* EN_port;
	uint16_t EN_pin;

	GPIO_TypeDef* RW_port;
	uint16_t RW_pin;

	GPIO_TypeDef* RS_port;
	uint16_t RS_pin;

	GPIO_TypeDef* D7_port;
	uint16_t D7_pin;

	GPIO_TypeDef* D6_port;
	uint16_t D6_pin;

	GPIO_TypeDef* D5_port;
	uint16_t D5_pin;

	GPIO_TypeDef* D4_port;
	uint16_t D4_pin;


	// Software initializations
	uint16_t delay;
	uint8_t displayCtrl;
}LCD_t;

void lcd_setDelay(LCD_t *lcd, uint16_t delay);
void lcd_pulseEn(const LCD_t *const lcd);
void lcd_writeCommand(const LCD_t *const lcd, uint8_t cmd);
void lcd_writeData(const LCD_t *const lcd, uint8_t data);
void lcd_clearDisplay(const LCD_t *const lcd);
void lcd_returnHome(const LCD_t *const lcd);
void lcd_setEntryMode(const LCD_t *const lcd, uint8_t increment, uint8_t display_shift);
void lcd_displayCtrl(LCD_t *const lcd, uint8_t display_on);
void lcd_showCursor(LCD_t *const lcd, uint8_t show_cursor);
void lcd_cursorBlink(LCD_t *const lcd, uint8_t blink_on);
void lcd_cursorShift(const LCD_t *const lcd, uint8_t direction);
void lcd_displayShift(const LCD_t *const lcd, uint8_t direction);
void lcd_setCursor(const LCD_t *const lcd, uint8_t row, uint8_t col);
void lcd_setPattern(const LCD_t *const lcd, uint8_t pattern_id, uint8_t *pattern);
void lcd_printMsg(const LCD_t *const lcd, char *msg);
void lcd_printChar(const LCD_t *const lcd, uint8_t pattern_id);
void lcd_reset(LCD_t *const lcd);
void lcd_init(const LCD_t *const lcd);

#endif /* INC_LCD_H_ */
