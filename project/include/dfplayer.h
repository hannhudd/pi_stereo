#define DF_SEQUENCE_LENGTH 10

// default bytes
#define DF_START_BIT 0x7E
#define DF_VERSION_INFO 0xFF
#define DF_STANDARD_LENGTH 0x06
#define DF_END_BIT 0xEF

// feedback bytes
#define DF_FEEDBACK_OFF 0x00
#define DF_FEEDBACK_ON 0x01

// control commands
#define DF_CMD_NEXT 0x01
#define DF_CMD_PREV 0x02
#define DF_CMD_TRACK 0x03 // 0-2999
#define DF_CMD_VOLINC 0x04
#define DF_CMD_VOLDEC 0x05
#define DF_CMD_VOLSET 0x06 // 0-30
#define DF_CMD_EQSET 0x07 // 0-5 (normal/pop/rock/jazz/classic/bass)
#define DF_CMD_MODESET 0x08 // 0-3 (repeat/folder repeat/single repeat/random)
#define DF_CMD_SOURCESET 0x09 // 0-4 (U/TF/AUX/SLEEP/FLASH)
#define DF_CMD_STANDBY 0x0A
#define DF_CMD_NORMAL 0x0B
#define DF_CMD_RESET 0x0C
#define DF_CMD_PLAY 0x0D
#define DF_CMD_PAUSE 0x0E
#define DF_CMD_FOLDER 0x0F // 1-10
#define DF_CMD_VOLADJ 0x10 // (did not use)
#define DF_CMD_REPEAT 0x11 // 0-1

// playback modes
#define DF_MODE_REPEAT 0
#define DF_MODE_FOLDER_REPEAT 1
#define DF_MODE_SINGLE_REPEAT 2
#define DF_MODE_RANDOM 3

// eq modes
#define DF_EQ_NORMAL 0
#define DF_EQ_POP 1
#define DF_EQ_ROCK 2
#define DF_EQ_JAZZ 3
#define DF_EQ_CLASSIC 4
#define DF_EQ_BASS 5

// playback sources
#define DF_SOURCE_U 0
#define DF_SOURCE_TF 1
#define DF_SOURCE_AUX 2
#define DF_SOURCE_SLEEP 3
#define DF_SOURCE_FLASH 4

void df_init(void);

void df_command(unsigned char command, unsigned int parameter);


// control track

void df_next(void);
void df_previous(void);
void df_play_track(unsigned int track);

// control volume

void df_increase_volume(void);
void df_decrease_volume(void);
void df_set_volume(unsigned int volume);
void df_set_equalization(unsigned int mode);

// control configuration

void df_set_playback_mode(unsigned int mode);
void df_set_playback_source(unsigned int source);
void df_standby_on(void);
void df_standby_off(void);
void df_reset(void);

// pause/play

void df_play(void);
void df_pause(void);

// other

void df_play_folder(unsigned int folder);
void df_repeat(void);
