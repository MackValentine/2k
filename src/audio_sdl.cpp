/*
 * This file is part of EasyRPG Player.
 *
 * EasyRPG Player is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EasyRPG Player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
 */

#include "system.h"

#if defined(USE_SDL) && !defined(HAVE_SDL_MIXER) && defined(SUPPORT_AUDIO)

#include <cassert>
#include <chrono>
#include <SDL.h>
#include <SDL_audio.h>
#include <SDL_version.h>
#include <SDL_thread.h>
#include <SDL_mutex.h>

#include "game_clock.h"
#include "audio_sdl.h"
#include "audio_midi.h"
#include "output.h"

using namespace std::chrono_literals;

namespace {
#if SDL_MAJOR_VERSION >= 2
	SDL_AudioDeviceID audio_dev_id = 0;
#endif
}

static int MidioutThreadMain(void* ptr) {
	SdlAudio* data = reinterpret_cast<SdlAudio*>(ptr);
	bool should_exit = false;
	Game_Clock::time_point start_ticks = Game_Clock::now();
	while (!should_exit) {
		auto ticks = Game_Clock::now();
		data->LockMidiOutMutex();

		should_exit = data->midiout_thread_exit;
		auto us = std::chrono::duration_cast<std::chrono::milliseconds>(ticks - start_ticks);
		data->UpdateMidiOut(us.count());

		data->UnlockMidiOutMutex();
		Game_Clock::SleepFor(1ms);
		start_ticks = ticks;
	}
	return 0;
}

void sdl_audio_callback(void* userdata, uint8_t* stream, int length) {
	// no mutex locking required, SDL does this before calling

	static_cast<GenericAudio*>(userdata)->Decode(stream, length);
}

AudioDecoder::Format sdl_format_to_format(Uint16 format) {
	switch (format) {
		case AUDIO_U8:
			return AudioDecoder::Format::U8;
		case AUDIO_S8:
			return AudioDecoder::Format::S8;
		case AUDIO_U16SYS:
			return AudioDecoder::Format::U16;
		case AUDIO_S16SYS:
			return AudioDecoder::Format::S16;
#if SDL_MAJOR_VERSION > 1
		case AUDIO_S32:
			return AudioDecoder::Format::S32;
		case AUDIO_F32:
			return AudioDecoder::Format::F32;
#endif
		default:
			assert(false);
	}

	return (AudioDecoder::Format)-1;
}

SdlAudio::SdlAudio() :
	GenericAudio()
{
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
		Output::Warning("Couldn't init audio: {}", SDL_GetError());
		return;
	}

	SDL_AudioSpec want = {};
	SDL_AudioSpec have = {};
	want.freq = 44100;
	want.format = AUDIO_S16;
	want.channels = 2;
	want.samples = 2048;
	want.callback = sdl_audio_callback;
	want.userdata = this;

#if SDL_MAJOR_VERSION >= 2
	audio_dev_id = SDL_OpenAudioDevice(nullptr, 0, &want, &have, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
	bool init_success = audio_dev_id > 0;
#else
	bool init_success = SDL_OpenAudio(&want, &have) >= 0;
#endif

	if (!init_success) {
		Output::Warning("Couldn't open audio: {}", SDL_GetError());
		return;
	}

	SetFormat(have.freq, sdl_format_to_format(have.format), have.channels);

	// Start Audio
#if SDL_MAJOR_VERSION >= 2
	SDL_PauseAudioDevice(audio_dev_id, 0);
#else
	SDL_PauseAudio(0);
#endif

#if SDL_MAJOR_VERSION >= 2
	// Start midiout polling thread, SDL2-only. Wii doesn't support MidiOut.
	//if (MidiOut::IsSupported()) {
		// TODO: Just because MidiOut is supported doesn't mean it's configured to be used.
		// Should we default to Fluidsynth if an easyrpg.soundfont is found, for example?
		// In which case this thread is wasteful...
		midiout_mutex = SDL_CreateMutex();
		midiout_thread = SDL_CreateThread(MidioutThreadMain, "MidioutThread", this);
		if (!midiout_thread) {
			Output::Warning("Couldn't start midiout thread: {}", SDL_GetError());
			return;
		}
	//}
#endif
}

SdlAudio::~SdlAudio() {
	if (midiout_thread) {
		LockMidiOutMutex();
		midiout_thread_exit = true;
		UnlockMidiOutMutex();
		SDL_WaitThread(midiout_thread, NULL);
		midiout_thread = nullptr;
		SDL_DestroyMutex(midiout_mutex);
		midiout_mutex = nullptr;
	}
#if SDL_MAJOR_VERSION >= 2
	SDL_CloseAudioDevice(audio_dev_id);
#else
	SDL_CloseAudio();
#endif
}

void SdlAudio::LockMutex() const {
#if SDL_MAJOR_VERSION >= 2
	SDL_LockAudioDevice(audio_dev_id);
#else
	SDL_LockAudio();
#endif
}

void SdlAudio::UnlockMutex() const {
#if SDL_MAJOR_VERSION >= 2
	SDL_UnlockAudioDevice(audio_dev_id);
#else
	SDL_UnlockAudio();
#endif
}

void SdlAudio::LockMidiOutMutex() const {
	if (SDL_LockMutex(midiout_mutex) != 0) {
		Output::Debug("SDL_LockMutex failure: {}", SDL_GetError());
	}
}

void SdlAudio::UnlockMidiOutMutex() const {
	if (SDL_UnlockMutex(midiout_mutex) != 0) {
		Output::Debug("SDL_UnlockMutex failure: {}", SDL_GetError());
	}
}

#endif
