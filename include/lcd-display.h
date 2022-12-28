// address and specs of lcd display
#define I2C_ADDRESS 0x27
#define NUM_ROWS 4
#define NUM_COLS 20

// first four bits of byte
#define HIGH_BITS 0xF0

// enable bit tells lcd display to read signal
#define ENABLE_BIT (1 << 2)

// bit to enable backlight
#define BACKLIGHT_BIT (1 << 3) // for backlight on
//#define BACKLIGHT_BIT (0) // for backlight off

// lcd command codes
#define LCD_CLEAR 0x01
#define LCD_RESET_CURSOR 0x02
#define LCD_CONTROL_CURSOR 0x04
#define LCD_CONTROL_DISPLAY 0x08
#define LCD_CONTROL_POSITION 0x10
#define LCD_CONTROL_FUNCTION 0x20

// flags for cursor
#define LCD_SET_CURSOR_RIGHT 0x02
#define LCD_CURSOR_AUTOSCROLL 0x01

// flags for display
#define LCD_DISPLAY_ON 0x04
#define LCD_CURSOR_ON 0x02
#define LCD_CURSOR_BLINK_ON 0x01

// flags for lcd function
#define LCD_8BIT_MODE 0x10
#define LCD_2LINE_MODE 0x08
#define LCD_5x10_DOTS 0x04

// flags for position
#define LCD_MOVE_CURSOR_RIGHT 0x04
#define LCD_MOVE_CURSOR_LEFT 0x00

// printf max bufsize
#define MAX_BUFSIZE 1024

typedef struct {
    int num_rows;
    int num_cols;
    char address;
    char display;
    char cursor;
    char function;
} lcd_t;

enum { LCD_CMD = 0, LCD_DATA = 1 }; // Byte Types

/*
 Initializes LCD module in 4-bit mode with the following settings
 1. 20x4 Display
 2. Display On
 3. Cursor Off
 4. Cursor Blink Off
 5. Cursor Moves Left-to-Right
 6. Cursor Autoscroll Off
 7. 4 Bit Mode
 8. 2 Line Mode
 9. 5x8 Dots Per Character
 */
void lcd_init(void);


void lcd_write(char byte, int type); // Writes Byte of Type type To LCD Display

/*Display Functions*/
void lcd_display_on(void); // Turns Display On
void lcd_display_off(void); // Turns Display Off
void lcd_cursor_on(void); // Turns Cursor On
void lcd_cursor_off(void); // Turns Cursor Off
void lcd_blink_on(void); // Turns Cursor Blink On
void lcd_blink_off(void); // Turns Cursor Blink Off

/*Cursor Functions*/
void lcd_cursor_right(void); // Sets Cursor Direction to Left-To-Right
void lcd_cursor_left(void);// Sets Cursor Direction to Right-to-Left
void lcd_autoscroll_on(void); // Sets Cursor Autoscroll On
void lcd_autoscroll_off(void); // Sets Cursor Autoscroll Off

/* Functionality Functions */
void lcd_8bit_mode(void); // Sets LCD to 8bit Mode
void lcd_4bit_mode(void); // Sets LCD to 4bit Mode
void lcd_2line_mode(void); // Sets LCD to 2 line Mode
void lcd_1line_mode(void); // Sets LCD to 1 line Mode
void lcd_5x10_dots_mode(void); // Sets LCD to 5x10 dots per character
void lcd_5x8_dots_mode(void); // Sets LCD to 5x8 dots per character

/* Position Functions */
void lcd_move_right(unsigned int steps); // Move Cursor steps Steps Right
void lcd_move_left(unsigned int steps); // Move Cursor steps Steps Left
void lcd_set_position(unsigned int row, unsigned int col); // Set Cursor Position to (row, col)

/* Other Functions */
void lcd_clear(void); // Clears LCD screen
void lcd_return_home(void); // Reset cursor to starting position
void lcd_write_char(char ch); // Writes char at current cursor position
void lcd_write_string(char *str); // Writes string at current position
int lcd_printf(const char *format, ...); // Printf with LCD
int lcd_printf_row(unsigned int row, const char * format, ...); // Printf with LCD only at certain row
