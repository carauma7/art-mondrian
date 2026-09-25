/*************************************************************************
 * 
 * sound.c
 * 
 * (c) 2021 J. G. Silva (django) <dparicarana@gmail.com>
 * 
 * "Snake game written in C - animations, retro menu, smart snake 
 * enemies, sound effects."
 * 
 * last modification: 2021-01-07
 * 
 ************************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "sound.h"
#include "utils.h"
#include "bass.h"

// Canal principal do dispositivo de áudio (áudios principais do jogo)
unsigned long int audio_mainchannel;

// Canal auxiliar para efeitos curtos sobre o áudio principal
unsigned long int audio_effectchannel;

// Canais separados para sons de tiro e coleta de alimentos
unsigned long int audio_shootchannel;
unsigned long int audio_shoot_foodchannel;
unsigned long int audio_shot_missedchannel;
unsigned long int audio_foodchannel;

// Canal reservado para vozes (ex: risada malévola), evita ser abafado por efeitos curtos
unsigned long int audio_voicechannel;
unsigned long int audio_gameover_musicchannel;
unsigned long int audio_gameover_voicechannel;

// Nome do último arquivo tocado no canal principal, usado por audio_is_track_playing
static char last_track[256] = "";

#define MAP_TRACK_COUNT 10
#define MAP_FADE_STEPS 8
#define MAP_FADE_STEP_MS 25

static char *map_tracks[MAP_TRACK_COUNT] = {
	AUDIO_MAP1, AUDIO_MAP2, AUDIO_MAP3, AUDIO_MAP4, AUDIO_MAP5,
	AUDIO_MAP6, AUDIO_MAP7, AUDIO_MAP8, AUDIO_MAP9, AUDIO_MAP10
};

static void audio_fade_volume(unsigned long int channel, float from, float to)
{
	int step;

	for (step = 1; step <= MAP_FADE_STEPS; step++)
	{
		float progress = (float) step / MAP_FADE_STEPS;
		float volume = from + (to - from) * progress;

		BASS_ChannelSetAttribute(channel, BASS_ATTRIB_VOL, volume);
		delay(MAP_FADE_STEP_MS);
	}
}

// Reproduz o áudio correspondente ao arquivo dado, atualizando *audiochannel
// com o handle do novo stream (senão o handle antigo é perdido e o canal
// não pode mais ser parado/reutilizado pelo chamador)
int audio_play(unsigned long int *audiochannel, char *filename, short repeat) 
{
	#ifdef SOUND_OFF
		(void) audiochannel;
		(void) filename;
		(void) repeat;
		return(EXIT_SUCCESS);
	#endif
	
	unsigned long int new_channel;

	BASS_ChannelStop(*audiochannel);

	if(!(new_channel = BASS_StreamCreateFile(
		FALSE, filename, 0, 0, repeat ? BASS_SAMPLE_LOOP : 0)))
//	&&if(!(new_channel = BASS_MusicLoad(
//		FALSE, filename, 0, 0, BASS_MUSIC_RAMP | BASS_SAMPLE_FLOAT | (BASS_SAMPLE_LOOP && repeat), 1)))
	{
		// Erro ao carregar arquivo...
		return(EXIT_FAILURE);
	} else {
		*audiochannel = new_channel;

		strncpy(last_track, filename, sizeof(last_track) - 1);
		last_track[sizeof(last_track) - 1] = '\0';

		// Inicia reprodução do áudio...
		BASS_ChannelPlay(*audiochannel, FALSE);
	}
	return(EXIT_SUCCESS);

}

// Troca a música do mapa com uma transição suave e repete as dez faixas.
int audio_play_map(int level)
{
	int track_index = (level - 1) % MAP_TRACK_COUNT;

	#ifdef SOUND_OFF
		(void) track_index;
		return(EXIT_SUCCESS);
	#endif

	if (audio_mainchannel)
	{
		audio_fade_volume(audio_mainchannel, 1.0f, 0.0f);
	}

	if (audio_play(&audio_mainchannel, map_tracks[track_index], TRUE) != EXIT_SUCCESS)
	{
		return(EXIT_FAILURE);
	}

	BASS_ChannelSetAttribute(audio_mainchannel, BASS_ATTRIB_VOL, 0.0f);
	audio_fade_volume(audio_mainchannel, 0.0f, 1.0f);

	return(EXIT_SUCCESS);
}

// Desabilita reprodução de áudio
void audio_stop(void) 
{
	#ifdef SOUND_OFF
		return;
	#endif

	BASS_Stop();
}

// Indica se o canal ainda está tocando (útil para esperar um áudio terminar)
int audio_is_playing(unsigned long int audiochannel) 
{
	#ifdef SOUND_OFF
		(void) audiochannel;
		return(FALSE);
	#endif

	return(BASS_ChannelIsActive(audiochannel) == BASS_ACTIVE_PLAYING);
}

// Indica se o arquivo dado é o que está tocando agora no canal principal
int audio_is_track_playing(const char *filename)
{
	#ifdef SOUND_OFF
		(void) filename;
		return(FALSE);
	#endif

	return(audio_is_playing(audio_mainchannel) && strcmp(last_track, filename) == 0);
}

// Habilita reprodução de áudio
void audio_resume(void)
{
	#ifdef SOUND_OFF
		return;
	#endif

	BASS_Start();
}

// Inicializa o módulo de áudio com as configurações padrão
int audio_initialize(void)
{
	#ifdef SOUND_OFF
		return(BASS_OK);
	#endif

	// Verifica se foi carregada uma versão correta da biblioteca BASS
	if (HIWORD(BASS_GetVersion()) != BASSVERSION)
	{
		return(BASS_ERROR_VERSION);
	}
	// Inicializa a biblioteca BASS
	if (!BASS_Init(-1, 44100, 0, NULL, NULL)) 
	{
		return(BASS_ERROR_INIT);
	}
	return(BASS_OK);
}

// Finaliza o módulo de áudio corretamente
void audio_terminate(void)
{
	#ifdef SOUND_OFF
		return;
	#endif

	BASS_Free();
}
