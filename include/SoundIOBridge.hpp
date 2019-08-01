#pragma once

#include <soundio/soundio.h>

#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <chrono>

#include "SoundInterface.hpp"


double elapsed_time = 0.0;

void write_callback(struct SoundIoOutStream *outstream, int frame_count_min, int frame_count_max) 
{
    const struct SoundIoChannelLayout *layout = &outstream->layout;
    struct SoundIoChannelArea *areas;

    double seconds_per_frame = 1.0 / outstream->sample_rate; // ?? 44100Hz?
    int frames_left = frame_count_max;

    // printf("%d %i %i\n", outstream->sample_rate, frame_count_min, frame_count_max);


    auto begin = std::chrono::high_resolution_clock::now();

    while (frames_left > 0) {
        int frame_count = frames_left;
        soundio_outstream_begin_write(outstream, &areas, &frame_count);

        if (!frame_count)
            break;

        for (int frame = 0; frame < frame_count; frame += 1) {
            double elapsed = elapsed_time + (double)frame * seconds_per_frame;

            float sample = ((SoundInterface*)outstream->userdata)->get_sample(elapsed);

            for (int channel = 0; channel < layout->channel_count; channel += 1) {
                float *ptr = (float*)(areas[channel].ptr + areas[channel].step * frame);
                *ptr = sample;
            }
        }

        elapsed_time += frame_count * seconds_per_frame;

        soundio_outstream_end_write(outstream);
        frames_left -= frame_count;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();

    if(duration > 10000000) std::cout << "TOO SLOW\n";
    // std::cout << duration << " ns for " << frame_count_max << " frames" << std::endl;
    // std::cout << 
    // 44100 Hz - 512 frames in write_callback - write_callback called 86 times per second
    // max time in function: 1/86 s = 11'627'906 ns
    // keeps accumulating -> need to empty vector when sound is over
    // but this is not the cause of the pb
    // if too slow we have grésillements
    // here its not grésillements

    
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

