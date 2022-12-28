#include "lcd-display.h"
#include "i2c.h"
#include "timer.h"
#include "malloc.h"
#include "printf.h"

static lcd_t *lcd = NULL;

/* Static Functions */
static void wait(void) { timer_delay_ms(1); }
static void lcd_reset_struct(void);
static void lcd_display_init(void);
static void lcd_cursor_init(void);
static void lcd_functionality_init(void);


/*----------------I2C Functions----------------*/
/* (Functions that directly call i2c.h) */

static void lcd_enable(char byte)
{
    char byte_or = byte | ENABLE_BIT;
    char byte_and = byte & ~ENABLE_BIT;
    
    wait();
    i2c_write(lcd->address, &byte_or, 1);
    wait();
    i2c_write(lcd->address, &byte_and, 1);
    wait();
}

void lcd_write(char byte, int type)
{
    char high_bits, low_bits;
    
    high_bits = (byte & HIGH_BITS) | type | BACKLIGHT_BIT;
    low_bits = ((byte << 4) & HIGH_BITS) | type | BACKLIGHT_BIT;
    
    i2c_write(lcd->address, &high_bits, 1);
    lcd_enable(high_bits);
    
    i2c_write(lcd->address, &low_bits, 1);
    lcd_enable(low_bits);
}

/*----------------LCD Functions----------------*/

/* Initialization */

void lcd_init(void)
{
    if (lcd != NULL) free(lcd);
    lcd = malloc(sizeof(lcd_t));
    
    lcd_reset_struct();
    
    /* Initialization Sequence */
    lcd_write(0x33, LCD_CMD);
    lcd_write(0x32, LCD_CMD);
    
    /* Initialize Display */
    lcd_display_init();
    
    /* Initialize Cursor */
    lcd_cursor_init();
    
    /* Initialize Functionality */
    lcd_functionality_init();
    
    /* Reset Screen */
    lcd_clear();
    lcd_return_home();
}

/* Display Functions */

void lcd_display_on(void)
{
    lcd->display |= LCD_DISPLAY_ON;
    lcd_write(LCD_CONTROL_DISPLAY | lcd->display, LCD_CMD);
}

void lcd_display_off(void)
{
    lcd->display &= ~LCD_DISPLAY_ON;
    lcd_write(LCD_CONTROL_DISPLAY | lcd->display, LCD_CMD);
}

void lcd_cursor_on(void)
{
    lcd->display |= LCD_CURSOR_ON;
    lcd_write(LCD_CONTROL_DISPLAY | lcd->display, LCD_CMD);
}

void lcd_cursor_off(void)
{
    lcd->display &= ~LCD_CURSOR_ON;
    lcd_write(LCD_CONTROL_DISPLAY | lcd->display, LCD_CMD);
}

void lcd_blink_on(void)
{
    lcd->display |= LCD_CURSOR_BLINK_ON;
    lcd_write(LCD_CONTROL_DISPLAY | lcd->display, LCD_CMD);
}

void lcd_blink_off(void)
{
    lcd->display &= ~LCD_CURSOR_BLINK_ON;
    lcd_write(LCD_CONTROL_DISPLAY | lcd->display, LCD_CMD);
}

/* Cursor Functions */

void lcd_cursor_right(void)
{
    lcd->cursor |= LCD_SET_CURSOR_RIGHT;
    lcd_write(LCD_CONTROL_CURSOR | lcd->cursor, LCD_CMD);
}

void lcd_cursor_left(void)
{
    lcd->cursor &= ~LCD_SET_CURSOR_RIGHT;
    lcd_write(LCD_CONTROL_CURSOR | lcd->cursor, LCD_CMD);
}

void lcd_autoscroll_on(void)
{
    lcd->cursor |= LCD_CURSOR_AUTOSCROLL;
    lcd_write(LCD_CONTROL_CURSOR | lcd->cursor, LCD_CMD);
}

void lcd_autoscroll_off(void)
{
    lcd->cursor &= ~LCD_CURSOR_AUTOSCROLL;
    lcd_write(LCD_CONTROL_CURSOR | lcd->cursor, LCD_CMD);
}

/* Functionality Functions */

void lcd_8bit_mode(void)
{
    lcd->function |= LCD_8BIT_MODE;
    lcd_write(LCD_CONTROL_FUNCTION | lcd->function, LCD_CMD);
}

void lcd_4bit_mode(void)
{
    lcd->function &= ~LCD_8BIT_MODE;
    lcd_write(LCD_CONTROL_FUNCTION | lcd->function, LCD_CMD);
}

void lcd_2line_mode(void)
{
    lcd->function |= LCD_2LINE_MODE;
    lcd_write(LCD_CONTROL_FUNCTION | lcd->function, LCD_CMD);
}

void lcd_1line_mode(void)
{
    lcd->function &= ~LCD_2LINE_MODE;
    lcd_write(LCD_CONTROL_FUNCTION | lcd->function, LCD_CMD);
}

void lcd_5x10_dots_mode(void)
{
    lcd->function |= LCD_5x10_DOTS;
    lcd_write(LCD_CONTROL_FUNCTION | lcd->function, LCD_CMD);
}

void lcd_5x8_dots_mode(void)
{
    lcd->function &= ~LCD_5x10_DOTS;
    lcd_write(LCD_CONTROL_FUNCTION | lcd->function, LCD_CMD);
}

/* Position Functions */

void lcd_move_right(unsigned int steps) {
    for (unsigned int i = 0; i < steps; i++) {
        lcd_write(LCD_CONTROL_POSITION | LCD_MOVE_CURSOR_RIGHT, LCD_CMD);
    }
}

void lcd_move_left(unsigned int steps) {
    for (unsigned int i = 0; i < steps; i++) {
        lcd_write(LCD_CONTROL_POSITION | LCD_MOVE_CURSOR_LEFT, LCD_CMD);
    }
}

void lcd_set_position(unsigned int row, unsigned int col) {
    lcd_return_home();
    if (row > (lcd->num_rows - 1) || col > (lcd->num_cols - 1)) return;
    
    // For some crazy reason, rows go 1-3-2-4... who designs this stuff?
    if (row == 1) {
        row = 2;
    } else if (row == 2) {
        row = 1;
    }
    
    unsigned int steps = row * lcd->num_cols + col;
    lcd_move_right(steps);
}

/* Other Functions */

void lcd_clear(void)
{
    lcd_write(LCD_CLEAR, LCD_CMD);
    timer_delay_ms(2); // slow function
}

void lcd_return_home(void)
{
    lcd_write(LCD_RESET_CURSOR, LCD_CMD);
    timer_delay_ms(2); // slow function

}

void lcd_write_char(char ch)
{
    lcd_write(ch, LCD_DATA);
}

void lcd_write_string(char *str)
{
    while (*str != '\0') {
        lcd_write_char(*(str++));
    }
}

int lcd_printf(const char *format, ...)
{
    char buf[MAX_BUFSIZE];
    va_list args;
    va_start(args, format); // create and initialize args list
    vsnprintf(buf, MAX_BUFSIZE, format, args); // format string
    va_end(args);
    
    int index = 0;
    while (buf[index] != '\0') {
        lcd_write_char(buf[index]); // process every char in string
        index++;
    }
    return index;
}

int lcd_printf_row(unsigned int row, const char * format, ...)
{
    lcd_set_position(row, 0);
    
    char buf[MAX_BUFSIZE];
    va_list args;
    va_start(args, format); // create and initialize args list
    vsnprintf(buf, MAX_BUFSIZE, format, args); // format string
    va_end(args);
    
    int index = 0;
    while (buf[index] != '\0' && index < 20) {
        lcd_write_char(buf[index]); // process every char in string
        index++;
    }
    return index;
}



/*----------Static Function Definitions----------*/

static void lcd_reset_struct(void)
{
    lcd->num_rows = NUM_ROWS;
    lcd->num_cols = NUM_COLS;
    lcd->address = I2C_ADDRESS;
    lcd->display = 0x00;
    lcd->cursor = 0x00;
    lcd->function = 0x00;
}

static void lcd_display_init(void)
{
    lcd_display_on();
    lcd_cursor_off();
    lcd_blink_off();
}

static void lcd_cursor_init(void)
{
    lcd_cursor_right();
    lcd_autoscroll_off();
}

static void lcd_functionality_init(void)
{
    lcd_4bit_mode();
    lcd_2line_mode();
    lcd_5x8_dots_mode();
}
