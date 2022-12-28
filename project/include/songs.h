typedef struct {
    const char * name;
    unsigned int length;
    unsigned int index;
} song_t;

typedef struct {
    const char * name;
    const char * artist;
    unsigned int year;
    unsigned int num_songs;
    const song_t * songs;
} album_t;

extern album_t const albums[];
