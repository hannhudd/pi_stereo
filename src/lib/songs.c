#include "songs.h"

song_t const songs_damn[] = {
    {"BLOOD.", 119, 1},
    {"DNA.", 186, 2},
    {"YAH.", 160, 3},
    {"ELEMENT.", 209, 4},
    {"FEEL.", 215, 5},
    {"LOYALTY.", 228, 6},
    {"PRIDE.", 272, 7},
    {"HUMBLE.", 177, 8},
    {"LUST.", 308, 9},
    {"LOVE.", 211, 10},
    {"XXX.", 254, 11},
    {"FEAR.", 415, 12},
    {"GOD.", 249, 13},
    {"DUCKWORTH.", 248, 14},
};

song_t const songs_currents[] = {
    {"Let It Happen", 466, 1},
    {"Nangs", 107, 2},
    {"The Moment", 255, 3},
    {"Yes I'm Changing", 270, 4},
    {"Eventually", 319, 5},
    {"Gossip", 55, 6},
    {"The Less I Know the Better", 218, 7},
    {"Past Life", 227, 8},
    {"Disciples", 108, 9},
    {"Cause I'm a Man", 241, 10},
    {"Reality in Motion", 252, 11},
    {"Love/Paranoia", 186, 12},
    {"New Person, Same Old Mistakes", 362, 13},
};

song_t const songs_plasticbeach[] = {
    {"Orchestral Intro", 67, 1},
    {"Welcome To The World Of The Plastic Beach", 209, 2},
    {"White Flag", 217, 3},
    {"Rhinestone Eyes", 194, 4},
    {"Stylo", 263, 5},
    {"Superfast Jellyfish", 170, 6},
    {"Empire Ants", 275, 7},
    {"Glitter Freeze", 236, 8},
    {"Some Kind of Nature", 175, 9},
    {"On Melancholy Hill", 227, 10},
    {"Broken", 191, 11},
    {"Sweepstakes", 311, 12},
    {"Plastic Beach", 221, 13},
    {"To Binge", 229, 14},
    {"Cloud Of Unknowing", 181, 15},
    {"Pirate Jet", 148, 16},
    {"Pirate's Progress", 236, 17},
    {"Three Hearts, Seven Seas, Twelve Moons", 130, 18},
};

song_t const songs_thenewabnormal[] = {
    {"The Adults Are Talking", 309, 1},
    {"Selfless", 222, 2},
    {"Brooklyn Bridge To Chorus", 236, 3},
    {"Bad Decisions", 294, 4},
    {"Eternal Summer", 375, 5},
    {"At The Door", 310, 6},
    {"Why Are Sundays So Depressing", 276, 7},
    {"Not The Same Anymore", 337, 8},
    {"Ode To The Mets", 352, 9},
};

album_t const albums[] = {
    {
        "DAMN.",
        "Kendrick Lamar",
        2017,
        14,
        songs_damn,
    },
    {
        "Currents",
        "Tame Impala",
        2015,
        13,
        songs_currents,
    },
    {
        "Plastic Beach",
        "Gorillaz",
        2010,
        18,
        songs_plasticbeach,
    },
    {
        "The New Abnormal",
        "The Strokes",
        2020,
        9,
        songs_thenewabnormal,
    }
};
