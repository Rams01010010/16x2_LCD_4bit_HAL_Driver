# 16x2_LCD_4bit_HAL_Driver
HAL Driver for interfacing 16x2 LCD in 4bit mode

### Usage:

- Copy the `.h` file inside `Core/Inc` folder and `.c` file inside `Core/Src` folder.
- In main.c file
    - Include the library "lcd.h".
      ```
      #include "lcd.h"
      ```
    - Create a variable and set pin values.
      ```
      LCD_t disp;
      disp.EN_port = EN_GPIO_Port;
      disp.EN_pin = EN_Pin;
      disp.RW_port = RW_GPIO_Port;
      disp.RW_pin = RW_Pin;
      disp.RS_port = RS_GPIO_Port;
      disp.RS_pin = RS_Pin;
      disp.D7_port = D7_GPIO_Port;
      disp.D7_pin = D7_Pin;
      disp.D6_port = D6_GPIO_Port;
      disp.D6_pin = D6_Pin;
      disp.D5_port = D5_GPIO_Port;
      disp.D5_pin = D5_Pin;
      disp.D4_port = D4_GPIO_Port;
      disp.D4_pin = D4_Pin;
      ```
    - Initialize the lcd with default settings.
      ```
      lcd_init(&disp);
      ```
      > If you want to configure LCD with custom settings, then call `lcd_reset(&disp)` and configure the settings using library functions.
    - Set the cursor and start printing.
      ```
      lcd_setCursor(&disp, 0, 0);
      lcd_printMsg(&disp, "Hello World!");
      ```
