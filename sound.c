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
#include <limits.h>
#ifdef _WIN32
#include <windows.h>
#endif
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
unsigned long int audio_deadchannel;
unsigned long int audio_hurtchannel;

// Canal reservado para vozes (ex: risada malévola), evita ser abafado por efeitos curtos
unsigned long int audio_voicechannel;
unsigned long int audio_gameover_musicchannel;
unsigned long int audio_gameover_voicechannel;

// Nome do último arquivo tocado no canal principal, usado por audio_is_track_playing
#ifndef SOUND_OFF
static char last_track[256] = "";
static int audio_initialized = 0;
static HMODULE audio_library = NULL;

typedef DWORD (WINAPI *BASSGetVersionFn)(void);
typedef BOOL (WINAPI *BASSInitFn)(int, DWORD, DWORD, HWND, const GUID *);
typedef BOOL (WINAPI *BASSFreeFn)(void);
typedef BOOL (WINAPI *BASSStartFn)(void);
typedef BOOL (WINAPI *BASSStopFn)(void);
typedef HSTREAM (WINAPI *BASSStreamCreateFileFn)(BOOL, const void *, QWORD, QWORD, DWORD);
typedef BOOL (WINAPI *BASSChannelStopFn)(DWORD);
typedef BOOL (WINAPI *BASSChannelPlayFn)(DWORD, BOOL);
typedef BOOL (WINAPI *BASSChannelSetAttributeFn)(DWORD, DWORD, float);
typedef DWORD (WINAPI *BASSChannelIsActiveFn)(DWORD);
typedef QWORD (WINAPI *BASSChannelGetLengthFn)(DWORD, DWORD);
typedef double (WINAPI *BASSChannelBytes2SecondsFn)(DWORD, QWORD);

typedef struct
{
	BASSGetVersionFn get_version;
	BASSInitFn init;
	BASSFreeFn free;
	BASSStartFn start;
	BASSStopFn stop;
	BASSStreamCreateFileFn stream_create_file;
	BASSChannelStopFn channel_stop;
	BASSChannelPlayFn channel_play;
	BASSChannelSetAttributeFn channel_set_attribute;
	BASSChannelIsActiveFn channel_is_active;
	BASSChannelGetLengthFn channel_get_length;
	BASSChannelBytes2SecondsFn channel_bytes_to_seconds;
} BASSApi;

static BASSApi bass_api;

static void audio_unload_library(void)
{
	if (audio_library)
	{
		FreeLibrary(audio_library);
		audio_library = NULL;
	}
	memset(&bass_api, 0, sizeof(bass_api));
}

static int audio_load_library(void)
{
	if (audio_library)
	{
		return(TRUE);
	}

	audio_library = LoadLibraryA("bass.dll");
	if (!audio_library)
	{
		audio_library = LoadLibraryA("lib\\bass.dll");
	}
	if (!audio_library)
	{
		return(FALSE);
	}

#define LOAD_BASS_API(member, symbol) \
	do { \
		FARPROC function = GetProcAddress(audio_library, symbol); \
		memcpy(&bass_api.member, &function, sizeof(bass_api.member)); \
	} while (0)
	LOAD_BASS_API(get_version, "BASS_GetVersion");
	LOAD_BASS_API(init, "BASS_Init");
	LOAD_BASS_API(free, "BASS_Free");
	LOAD_BASS_API(start, "BASS_Start");
	LOAD_BASS_API(stop, "BASS_Stop");
	LOAD_BASS_API(stream_create_file, "BASS_StreamCreateFile");
	LOAD_BASS_API(channel_stop, "BASS_ChannelStop");
	LOAD_BASS_API(channel_play, "BASS_ChannelPlay");
	LOAD_BASS_API(channel_set_attribute, "BASS_ChannelSetAttribute");
	LOAD_BASS_API(channel_is_active, "BASS_ChannelIsActive");
	LOAD_BASS_API(channel_get_length, "BASS_ChannelGetLength");
	LOAD_BASS_API(channel_bytes_to_seconds, "BASS_ChannelBytes2Seconds");
#undef LOAD_BASS_API

	if (!bass_api.get_version || !bass_api.init || !bass_api.free ||
		!bass_api.start || !bass_api.stop || !bass_api.stream_create_file ||
		!bass_api.channel_stop || !bass_api.channel_play ||
		!bass_api.channel_set_attribute || !bass_api.channel_is_active ||
		!bass_api.channel_get_length || !bass_api.channel_bytes_to_seconds)
	{
		audio_unload_library();
		return(FALSE);
	}

	return(TRUE);
}

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

		bass_api.channel_set_attribute(channel, BASS_ATTRIB_VOL, volume);
		delay(MAP_FADE_STEP_MS);
	}
}
#endif

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
	#else
	unsigned long int new_channel;

	if (!audio_initialized)
	{
		return(EXIT_SUCCESS);
	}

	bass_api.channel_stop(*audiochannel);

	if(!(new_channel = bass_api.stream_create_file(
		FALSE, filename, 0, 0, repeat ? BASS_SAMPLE_LOOP : 0)))
//	&&if(!(new_channel = BASS_MusicLoad(
//		FALSE, filename, 0, 0, BASS_MUSIC_RAMP | BASS_SAMPLE_FLOAT | (BASS_SAMPLE_LOOP && repeat), 1)))
	{
		// Erro ao carregar arquivo...
		return(EXIT_FAILURE);
	} else {
		*audiochannel = new_channel;

		if (audiochannel == &audio_mainchannel)
		{
			strncpy(last_track, filename, sizeof(last_track) - 1);
			last_track[sizeof(last_track) - 1] = '\0';
		}

		// Inicia reprodução do áudio...
		bass_api.channel_play(*audiochannel, FALSE);
	}
	return(EXIT_SUCCESS);
	#endif

}

// Troca a música do mapa com uma transição suave e repete as dez faixas.
int audio_play_map(int level)
{
	#ifdef SOUND_OFF
		(void) level;
		return(EXIT_SUCCESS);
	#else
	int track_index = (level - 1) % MAP_TRACK_COUNT;

	if (!audio_initialized)
	{
		return(EXIT_SUCCESS);
	}

	if (audio_mainchannel)
	{
		audio_fade_volume(audio_mainchannel, 1.0f, 0.0f);
	}

	if (audio_play(&audio_mainchannel, map_tracks[track_index], TRUE) != EXIT_SUCCESS)
	{
		return(EXIT_FAILURE);
	}

	bass_api.channel_set_attribute(audio_mainchannel, BASS_ATTRIB_VOL, 0.0f);
	audio_fade_volume(audio_mainchannel, 0.0f, 1.0f);

	return(EXIT_SUCCESS);
	#endif
}

// Desabilita reprodução de áudio
void audio_stop(void) 
{
	#ifdef SOUND_OFF
		return;
	#else
	if (audio_initialized)
	{
		bass_api.stop();
	}
	#endif
}

// Indica se o canal ainda está tocando (útil para esperar um áudio terminar)
int audio_is_playing(unsigned long int audiochannel) 
{
	#ifdef SOUND_OFF
		(void) audiochannel;
		return(FALSE);
	#else
	return(audio_initialized &&
		bass_api.channel_is_active(audiochannel) == BASS_ACTIVE_PLAYING);
	#endif
}

// Indica se o arquivo dado é o que está tocando agora no canal principal
int audio_is_track_playing(const char *filename)
{
	#ifdef SOUND_OFF
		(void) filename;
		return(FALSE);
	#else
	return(audio_is_playing(audio_mainchannel) && strcmp(last_track, filename) == 0);
	#endif
}

unsigned int audio_get_duration_ms(unsigned long int audiochannel)
{
	#ifdef SOUND_OFF
		(void) audiochannel;
		return(0);
	#else
	if (!audio_initialized)
	{
		return(0);
	}

	QWORD length;
	double seconds;

	length = bass_api.channel_get_length(audiochannel, BASS_POS_BYTE);
	seconds = bass_api.channel_bytes_to_seconds(audiochannel, length);
	if (seconds <= 0.0 || seconds >= (double) UINT_MAX / 1000.0)
	{
		return(0);
	}

	return((unsigned int) (seconds * 1000.0));
	#endif
}

// Habilita reprodução de áudio
void audio_resume(void)
{
	#ifdef SOUND_OFF
		return;
	#else
	if (audio_initialized)
	{
		bass_api.start();
	}
	#endif
}

// Inicializa o módulo de áudio com as configurações padrão
int audio_initialize(void)
{
	#ifdef SOUND_OFF
		return(BASS_OK);
	#else
	if (audio_initialized)
	{
		return(BASS_OK);
	}

	if (!audio_load_library())
	{
		return(BASS_ERROR_INIT);
	}

	// Verifica se foi carregada uma versão correta da biblioteca BASS
	if (HIWORD(bass_api.get_version()) != BASSVERSION)
	{
		audio_unload_library();
		return(BASS_ERROR_VERSION);
	}
	// Inicializa a biblioteca BASS
	if (!bass_api.init(-1, 44100, 0, NULL, NULL))
	{
		audio_unload_library();
		return(BASS_ERROR_INIT);
	}

	audio_initialized = 1;
	return(BASS_OK);
	#endif
}

// Finaliza o módulo de áudio corretamente
void audio_terminate(void)
{
	#ifdef SOUND_OFF
		return;
	#else
	if (audio_initialized)
	{
		bass_api.free();
		audio_initialized = 0;
		audio_mainchannel = 0;
		last_track[0] = '\0';
	}
	audio_unload_library();
	#endif
}
