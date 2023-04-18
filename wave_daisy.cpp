#include "wave_daisy.h"

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	toggle.Debounce();
	updateTable = toggle.FallingEdge();

	for (size_t i = 0; i < size; i++)
	{
		OUT_L[i] = OUT_R[i] = osc.Process() * adsr.Process(noteOn != (uint8_t)-1);
	}
}

void Init()
{
	/** Harware initialization */
	hw.Init();
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	toggle.Init(hw.B8);

	/** Oscillator Initialization */
	// sine wave in buffer one
	float phase = TWOPI_F / (float)WAVE_SIZE;
	for (size_t i = 0; i < WAVE_SIZE; i++)
	{
		wave_buf_one.samples[i] = sinf(i * phase);
	}
	// sawtooth in buffer two
	float t = WAVE_SIZE / 2;
	float step = 1 / t;
	for (size_t i = 0; i < WAVE_SIZE; i++)
	{
		if (i <= t)
			wave_buf_two.samples[i] = i * step;
		else
			wave_buf_two.samples[i] = (i - t) * step - 1;
	}
	updateBuffer = &wave_buf_two;
	osc.Init(hw.AudioSampleRate(), &wave_buf_one);

	/** USB Midi Initialization */
	MidiUsbHandler::Config midi_cfg;
	midi_cfg.transport_config.periph = MidiUsbTransport::Config::INTERNAL;
	midi.Init(midi_cfg);

	noteOn = -1;
	adsr.Init(hw.AudioSampleRate());
}

int main(void)
{
	Init();

	hw.StartAudio(AudioCallback);
	while (1)
	{
		/** Listen to MIDI for new changes */
		midi.Listen();
		while (midi.HasEvents())
		{
			auto msg = midi.PopEvent();
			switch (msg.type)
			{
			case NoteOn:
				NoteOnMsg(msg);
				break;
			case NoteOff:
				NoteOffMsg(msg);
				break;
			case ControlChange:
				CCMsg(msg);
				break;
			default:
				break;
			}
		}

		/** Change wave */
		if (updateTable)
		{
			updateBuffer = osc.SetWaveform(updateBuffer);
			updateTable = false;
		}
	}
}

void NoteOnMsg(MidiEvent msg)
{
	auto note_msg = msg.AsNoteOn();
	if (note_msg.velocity != 0)
	{
		/** change the frequency of the oscillator */
		noteOn = note_msg.note;
		osc.SetFreq(daisysp::mtof(noteOn));
		hw.SetLed(true);
	}
	else
	{
		if (noteOn == note_msg.note)
		{
			noteOn = -1;
			hw.SetLed(false);
		}
	}
}

void NoteOffMsg(MidiEvent msg)
{
	auto note_msg = msg.AsNoteOff();
	if (noteOn == note_msg.note)
	{
		noteOn = -1;
		hw.SetLed(false);
	}
}

void CCMsg(MidiEvent msg)
{
	auto cc_msg = msg.AsControlChange();
	if (cc_msg.control_number == 7)
	{
		osc.SetAmp(midiMap(cc_msg.value, 0.001f, 0.7f));
	}
}
