#pragma once

#include <soundio/soundio.h>

#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>


struct Wrapper {
    std::function<float(float,float)> function;
    float frequency;
};

static float elapsedTime = 0.0f;

void write_callback(struct SoundIoOutStream *outstream, int frameCountMin, int frameCountMax) 
{
    const struct SoundIoChannelLayout *layout = &outstream->layout;
    struct SoundIoChannelArea *areas;

    float secondsPerFrame = 1.0f / outstream->sample_rate;
    int framesLeft = frameCountMax;

    float frequency = ((Wrapper*)outstream->userdata)->frequency;

    while (framesLeft > 0) {
        int frameCount = framesLeft;
        soundio_outstream_begin_write(outstream, &areas, &frameCount);

        if (!frameCount)
            break;

        for (int frame = 0; frame < frameCount; frame += 1) {
            float sample = ((Wrapper*)outstream->userdata)->function(elapsedTime, frequency);
            elapsedTime += secondsPerFrame;
            for (int channel = 0; channel < layout->channel_count; channel += 1) {
                float *ptr = (float*)(areas[channel].ptr + areas[channel].step * frame);
                *ptr = sample;
            }
        }

        soundio_outstream_end_write(outstream);
        framesLeft -= frameCount;
    }
}

class SoundMaker {

public:
    SoundMaker()
    {
        soundio = soundio_create();
        soundio_connect(soundio);
        soundio_flush_events(soundio);

        int default_out_device_index = soundio_default_output_device_index(soundio);
        device = soundio_get_output_device(soundio, default_out_device_index);

        outstream = soundio_outstream_create(device);
        outstream->format = SoundIoFormatFloat32NE;
        outstream->write_callback = write_callback;            
        outstream->userdata = new Wrapper;

        soundio_outstream_open(outstream);
        soundio_outstream_pause(outstream, true);
        soundio_outstream_start(outstream);
        soundio_outstream_pause(outstream, true);
    }

    ~SoundMaker() {
        soundio_outstream_destroy(outstream);
        soundio_device_unref(device);
        soundio_destroy(soundio);
    }

public:
    void setSoundFunction(std::function<float(float,float)> getAmplitude) {
        if (getAmplitude == nullptr) {
            soundio_outstream_pause(outstream, true);
        }
        else {
            ((Wrapper*)outstream->userdata)->function = getAmplitude;
            soundio_outstream_pause(outstream, false);
        }        
    }
    void setSoundFrequency(float frequency) {
        ((Wrapper*)outstream->userdata)->frequency = frequency;              
    }

private:
    struct SoundIo *soundio;
    struct SoundIoDevice *device;
    struct SoundIoOutStream *outstream;
};

