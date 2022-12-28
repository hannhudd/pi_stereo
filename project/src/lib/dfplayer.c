#include "dfplayer.h"
#include "software-uart.h"
#include "printf.h"

void df_init(void)
{
    soft_uart_init(9600);
}

void df_command(unsigned char command, unsigned int parameter)
{
    unsigned char param_high = (unsigned char)(parameter >> 8);
    unsigned char param_low = (unsigned char)(parameter & 0xff);
    unsigned int checksum = 0 - DF_VERSION_INFO - DF_STANDARD_LENGTH - command - DF_FEEDBACK_OFF - param_high - param_low;
    
    unsigned char sequence[DF_SEQUENCE_LENGTH] = {
        DF_START_BIT,
        DF_VERSION_INFO,
        DF_STANDARD_LENGTH,
        command,
        DF_FEEDBACK_OFF,
        param_high,
        param_low,
        (unsigned char)(checksum >> 8),
        (unsigned char)(checksum & 0xff),
        DF_END_BIT
    };
    
    for (int i = 0; i < DF_SEQUENCE_LENGTH; i++) {
        soft_uart_putchar(sequence[i]);
    }
}

// control track

void df_next(void)
{
    df_command(DF_CMD_NEXT, 0);
}

void df_previous(void)
{
    df_command(DF_CMD_PREV, 0);
}

void df_play_track(unsigned int track)
{
    df_command(DF_CMD_TRACK, track);
}

// control volume

void df_increase_volume(void)
{
    df_command(DF_CMD_VOLINC, 0);
}

void df_decrease_volume(void)
{
    df_command(DF_CMD_VOLDEC, 0);
}

void df_set_volume(unsigned int volume)
{
    df_command(DF_CMD_VOLSET, volume);
}

void df_set_equalization(unsigned int mode)
{
    df_command(DF_CMD_EQSET, mode);
}

// control configuration

void df_set_playback_mode(unsigned int mode)
{
    df_command(DF_CMD_MODESET, mode);
}

void df_set_playback_source(unsigned int source)
{
    df_command(DF_CMD_SOURCESET, source);
}

void df_standby_on(void)
{
    df_command(DF_CMD_STANDBY, 0);
}

void df_standby_off(void)
{
    df_command(DF_CMD_NORMAL, 0);
}

void df_reset(void)
{
    df_command(DF_CMD_RESET, 0);
}

// pause/play

void df_play(void)
{
    df_command(DF_CMD_PLAY, 0);
}

void df_pause(void)
{
    df_command(DF_CMD_PAUSE, 0);
}

// other

void df_play_folder(unsigned int folder)
{
    df_command(DF_CMD_FOLDER, folder);
}

void df_repeat(void)
{
    df_command(DF_CMD_REPEAT, 0);
}
