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

// Headers
#include "audio_midi.h"
#include "audio_decoder_midi.h"
#include "decoder_fluidsynth.h"
#include "decoder_fmmidi.h"
#include "decoder_wildmidi.h"
#include "output.h"
#ifdef _WIN32
#include "platform/windows/midiout_device_win32.h"
#elif __APPLE__
#include "platform/macos/midiout_device_coreaudio.h"
#endif

#ifdef USE_AUDIO_RESAMPLER
#include "audio_resampler.h"
#endif

void MidiDecoder::GetFormat(int& freq, AudioDecoderBase::Format& format, int& channels) const {
	freq = frequency;
	format = AudioDecoderBase::Format::S16;
	channels = 2;
}

bool MidiDecoder::SetFormat(int frequency, AudioDecoderBase::Format format, int channels) {
	if (frequency != EP_MIDI_FREQ || channels != 2 || format != AudioDecoderBase::Format::S16)
		return false;

	return true;
}

std::unique_ptr<AudioDecoderMidi> MidiDecoder::CreateMidiOut(Filesystem_Stream::InputStream& stream) {
	std::unique_ptr<AudioDecoderMidi> midiout;
	char magic[4] = { 0 };
	if (!stream.ReadIntoObj(magic)) {
		return nullptr;
	}
	stream.seekg(0, std::ios::beg);
	if (strncmp(magic, "MThd", 4) != 0) {
		return nullptr;
	}
#ifdef _WIN32
	auto dec = std::make_unique<Win32MidiOutDevice>();
	midiout = std::make_unique<AudioDecoderMidi>(std::move(dec));
	/*FIXME if (!device->IsOK()) {
		return nullptr;
	}*/
#endif
#ifdef __APPLE__
	auto dec = std::make_unique<CoreAudioMidiOutDevice>();
	midiout = std::make_unique<AudioDecoderMidi>(std::move(dec));
	/*if (!device->IsOK()) {
		return nullptr;
	}*/
#endif
	return midiout;
}

static struct {
	bool fluidsynth = true;
	bool wildmidi = true;
} works;

std::unique_ptr<AudioDecoderBase> MidiDecoder::CreateMidiPlayer(Filesystem_Stream::InputStream& stream, bool resample) {
	std::unique_ptr<AudioDecoderBase> mididec;
	std::string error_message;

#if defined(HAVE_FLUIDSYNTH) || defined(HAVE_FLUIDLITE)
	if (works.fluidsynth && FluidSynthDecoder::Initialize(error_message)) {
		auto dec = std::make_unique<FluidSynthDecoder>();
		mididec = std::make_unique<AudioDecoderMidi>(std::move(dec));
		resample = false;
	}
	else if (!mididec && works.fluidsynth) {
		Output::Debug("{}", error_message);
		works.fluidsynth = false;
	}
#endif
#ifdef HAVE_WILDMIDI
	if (!mididec && works.wildmidi && WildMidiDecoder::Initialize(error_message)) {
		auto dec = std::make_unique<WildMidiDecoder>();
		mididec = std::make_unique<AudioDecoderMidi>(std::move(dec));
	}
	else if (!mididec && works.wildmidi) {
		Output::Debug("{}", error_message);
		works.wildmidi = false;
	}
#endif
#if WANT_FMMIDI == 1
	if (!mididec) {
		auto dec = std::make_unique<FmMidiDecoder>();
		mididec = std::make_unique<AudioDecoderMidi>(std::move(dec));
		resample = false;
	}
#endif

#ifdef USE_AUDIO_RESAMPLER
	if (mididec && resample) {
		mididec = std::make_unique<AudioResampler>(std::move(mididec));
	}
#endif

	return mididec;
}
