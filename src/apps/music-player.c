#include <stdbool.h>
#include "timer.h"
#include "gpio.h"
#include "gpioextra.h"
#include "i2c.h"
#include "lcd-display.h"
#include "dfplayer.h"
#include "songs.h"
#include "pi.h"
#include "uart.h"
#include "printf.h"
#include "rfid.h"

#define READ 0 // runs program
//#define WRITE 1 // to write to cards

#define TICKS_IN_SEC 1000000

#define SOURCE_DEFAULT (DF_SOURCE_U)
#define VOL_DEFAULT (20)
#define EQ_DEFAULT (DF_EQ_NORMAL)

#define SKIP_BUTTON GPIO_PIN16
#define VOLUME_UP GPIO_PIN23
#define VOLUME_DOWN GPIO_PIN24

void dfplayer_config(void)
{
    df_set_playback_source(SOURCE_DEFAULT);
    df_set_volume(VOL_DEFAULT);
    df_set_equalization(EQ_DEFAULT);
}

void player_init(void)
{
    gpio_init();
    timer_init();
    i2c_init();
    lcd_init();
    df_init();
    dfplayer_config();
    PCD_Init();
    gpio_set_pullup(SKIP_BUTTON);
    gpio_set_pullup(VOLUME_UP);
    gpio_set_pullup(VOLUME_DOWN);
}

void play_album(unsigned int album_index)
{
    unsigned int count = 1;
    for (int i = 0; i < album_index; i++) {\
        count += albums[i].num_songs;
    }
    df_play_track(count);
}

void update_display(album_t album, song_t song)
{
    lcd_clear();
    lcd_printf_row(0, song.name);
    lcd_printf_row(1, album.artist);
    lcd_printf_row(2, "%s (%d)", album.name, album.year);
}

void update_time(unsigned int ticks, song_t song)
{
    unsigned int time_elapsed = (timer_get_ticks() - ticks) / TICKS_IN_SEC;
    lcd_printf_row(3, "%02d:%02d - %02d:%02d", time_elapsed / 60, time_elapsed % 60, song.length / 60, song.length % 60 );
}

void main(void)
{
#ifdef READ
    player_init();

    unsigned int album_index;
    unsigned int song_index;
    album_t album;
    unsigned int ticks;
    unsigned int new_album;

    while (true) {
        if (wake_select_readnum(&album_index)) break;
    }

    while (true) {
        new_album = 0;
        song_index = 0;
        album = albums[album_index];
        play_album(album_index);

        ticks = timer_get_ticks();

        while (song_index < album.num_songs) {
            song_t song = album.songs[song_index];
            update_display(album, song);
            while (timer_get_ticks() - ticks < song.length * TICKS_IN_SEC) {
                update_time(ticks, song);
                if (!gpio_read(SKIP_BUTTON)) {
                    break; // skip to next track
                }
                if (!gpio_read(VOLUME_UP)) {
                    df_increase_volume(); // increase volume
                }
                if (!gpio_read(VOLUME_DOWN)) {
                    df_decrease_volume(); // decrease volume
                }
                if (wake_select_readnum(&album_index)) {
                    new_album = 1; // new card has been read
                    break;
                }
            }
            if (new_album) break; // play new album
            song_index++;
            if (song_index < album.num_songs) df_next();
            ticks = timer_get_ticks();
        }
    }
#elif WRITE
    unsigned char write_value = 3; // change for different write values
    PCD_Init();
    wake_select_writenum(write_value);
    printf("Value Written: %d", write_value);
    uart_putchar(EOT);
    pi_reboot();
#endif
}
