#pragma once

#include <soundio/soundio.h>

#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "SoundInterface.hpp"


float elapsed_time = 0.0f;

void write_callback(struct SoundIoOutStream *outstream, int frame_count_min, int frame_count_max) 
{
    const struct SoundIoChannelLayout *layout = &outstream->layout;
    struct SoundIoChannelArea *areas;

    float seconds_per_frame = 1.0f / outstream->sample_rate;
    int frames_left = frame_count_max;

    while (frames_left > 0) {
        int frame_count = frames_left;
        soundio_outstream_begin_write(outstream, &areas, &frame_count);

        if (!frame_count)
            break;

        for (int frame = 0; frame < frame_count; frame += 1) {
            float elapsed = elapsed_time + frame * seconds_per_frame;
            float sample = ((SoundInterface*)outstream->userdata)->get_sample(elapsed);

            for (int channel = 0; channel < layout->channel_count; channel += 1) {
                float *ptr = (float*)(areas[channel].ptr + areas[channel].step * frame);
                *ptr = sample;
            }
        }

        elapsed_time = fmod(elapsed_time + frame_count * seconds_per_frame, 1.0);

        soundio_outstream_end_write(outstream);
        frames_left -= frame_count;
    }
}

class SoundIOBridge {

public:
    SoundIOBridge(SoundInterface* sound_interface)
    {
        soundio = soundio_create();
        soundio_connect(soundio);
        soundio_flush_events(soundio);

        int default_out_device_index = soundio_default_output_device_index(soundio);
        device = soundio_get_output_device(soundio, default_out_device_index);

        outstream = soundio_outstream_create(device);
        outstream->format = SoundIoFormatFloat32NE;
        outstream->write_callback = write_callback;            
        outstream->userdata = sound_interface;

        soundio_outstream_open(outstream);
        soundio_outstream_start(outstream);
        soundio_outstream_pause(outstream, false);
    }

    ~SoundIOBridge() {
        soundio_outstream_destroy(outstream);
        soundio_device_unref(device);
        soundio_destroy(soundio);
    }

private:
    struct SoundIo *soundio;
    struct SoundIoDevice *device;
    struct SoundIoOutStream *outstream;
};

