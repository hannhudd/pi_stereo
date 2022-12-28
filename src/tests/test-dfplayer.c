#include "dfplayer.h"
#include "gpio.h"
#include "timer.h"
#include "pi.h"
#include "printf.h"
#include "lcd-display.h"
#include "i2c.h"

void main(void)
{
    gpio_init();
    df_init();
    i2c_init();
    lcd_init();
    unsigned int timer;
    lcd_printf_row(1, "Track 1: Blood");
    df_play_track(1);
    timer = timer_get_ticks();
    while (1) {
        if (timer_get_ticks() - timer > 118000000) {
            lcd_printf_row(1, "Track 2: DNA");
            break;
        }
    }
    pi_reboot();
}
