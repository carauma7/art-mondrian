/*************************************************************************
 * 
 * sound.h
 * 
 * (c) 2021 J. G. Silva (django) <dparicarana@gmail.com>
 * 
 * "Snake game written in C - animations, retro menu, smart snake 
 * enemies, sound effects."
 * 
 * last modification: 2021-01-07
 * 
 ************************************************************************/
 
#ifndef SOUND_H
#define SOUND_H

#define	SOUND

#define FALSE	0
#define TRUE	1

#define AUDIO_INTRO				"audio/intro.mp3"
#define AUDIO_MENU_MAIN 		"audio/mainmenu.mp3"
#define AUDIO_DJANGO			"audio/django.mp3"
#define AUDIO_TEFX_ADDCHAR		"audio/addchar.mp3"
#define AUDIO_TEFX_SLIDING		"audio/sliding.mp3"
#define AUDIO_TEFX_SLIDINGTOC	"audio/slidingtoc.mp3"
#define AUDIO_FOOD1				"audio/food1.mp3"
#define AUDIO_MAP1				"audio/map1.mp3"
#define AUDIO_MAP2                "audio/map2.mp3"
#define AUDIO_MAP3                "audio/map3.mp3"
#define AUDIO_MAP4                "audio/map4.mp3"
#define AUDIO_MAP5                "audio/map5.mp3"
#define AUDIO_MAP6                "audio/map6.mp3"
#define AUDIO_MAP7                "audio/map7.mp3"
#define AUDIO_MAP8                "audio/map8.mp3"
#define AUDIO_MAP9                "audio/map9.mp3"
#define AUDIO_MAP10               "audio/map10.mp3"
#define AUDIO_EVILLAUGH			"audio/evil-laugh.mp3"
#define AUDIO_DEAD              "audio/dead.mp3"
#define AUDIO_GLITCH            "audio/glitch.mp3"
#define AUDIO_SHOOT             "audio/shoot.mp3"
#define AUDIO_BELLS             "audio/bells.mp3"
#define AUDIO_SHOOT_FOOD        "audio/shoot-food.mp3"
#define AUDIO_GOODBYE           "audio/goodbye.mp3"
#define AUDIO_LEVELUP           "audio/levelup.mp3"
#define AUDIO_PAUSE             "audio/pause.mp3"

extern unsigned long int audio_mainchannel;
extern unsigned long int audio_effectchannel;
extern unsigned long int audio_voicechannel;

int audio_initialize(void);
void audio_terminate(void);
int audio_play(unsigned long int *audiochannel, char *filename, short repeat);
int audio_is_playing(unsigned long int audiochannel);
int audio_is_track_playing(const char *filename);
int audio_play_map(int level);
void audio_stop(void);
void audio_resume(void);

#endif /* SOUND_H */
