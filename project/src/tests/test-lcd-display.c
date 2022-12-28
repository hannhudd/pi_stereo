#include "i2c.h"
#include "printf.h"
#include "timer.h"
#include "lcd-display.h"

#define I2C_ADDRESS 0x27

void test_display(void)
{
    char * str = "Hello!";
    while (*str != '\0') {
        lcd_write(*(str++), LCD_DATA);
    }
    timer_delay(3);
    
    printf("\nDisplay Off\n");
    lcd_display_off();
    timer_delay(3);
    
    printf("\nDisplay On\n");
    lcd_display_on();
    timer_delay(3);
    
    printf("\nCursor On\n");
    lcd_cursor_on();
    timer_delay(3);
    
    printf("\nCursor Blink On\n");
    lcd_blink_on();
    timer_delay(3);
    
    printf("\nCursor Blink Off\n");
    lcd_blink_off();
    timer_delay(3);
    
    printf("\nCursor Off\n");
    lcd_cursor_off();
    timer_delay(3);
}

void test_cursor(void)
{
    lcd_clear();
    lcd_return_home();
    
    printf("\nCursor Right : Printing 'Hello'\n");
    lcd_cursor_right();
    lcd_write_string("Hello");
    timer_delay(3);
    
    printf("\nCursor Left : Printing 'Wow'\n");
    lcd_cursor_left();
    lcd_write_string("Wow");
    timer_delay(3);
    
    printf("\nCursor Right w/ Autoscroll : Printing 'Hello'\n");
    lcd_cursor_right();
    lcd_autoscroll_on();
    lcd_write_string("Hello");
    timer_delay(3);
    
    lcd_autoscroll_off();
}

void test_position(void)
{
    lcd_clear();
    lcd_return_home();
    
    printf("\nCircles in Corners\n");
    lcd_set_position(0, 0);
    lcd_write_char('O');
    lcd_set_position(0, 19);
    lcd_write_char('O');
    lcd_set_position(3, 19);
    lcd_write_char('O');
    lcd_set_position(3, 0);
    lcd_write_char('O');
    timer_delay(3);
    
    lcd_clear();
    printf("\nDiagonal\n");
    lcd_set_position(0,0);
    lcd_write_char('\\');
    lcd_set_position(1,1);
    lcd_write_char('\\');
    lcd_set_position(2,2);
    lcd_write_char('\\');
    lcd_set_position(3,3);
    lcd_write_char('\\');
}

void test_lcd_printf(void)
{
    lcd_clear();
    lcd_return_home();
    
    unsigned int my_age = 19;
    unsigned int not_my_age = 12;
    lcd_printf("I am %d. I am not %d!!!!", my_age, not_my_age);
    timer_delay(3);
    
    lcd_clear();
    for (int i = 0; i < 4; i++) {
        lcd_printf_row(i, "This is Row %d", i);
    }
    timer_delay(3);
    
    lcd_clear(); // Test overflow
    lcd_printf_row(1, "This is meant to %s!!!!", "overflow and cut off");
}

void main(void)
{
    i2c_init();
    lcd_init();
    
//    test_display();
//    test_cursor();
//    test_position();
    test_lcd_printf();
    
    while (1);
}
