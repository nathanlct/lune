#include "SoundInterface.hpp"



float SoundInterface::get_sample(double elapsed) {
    float amplitude = 0.3f;
    float sample = 0.0f;

    for(auto& sound: sounds) {
        sample += amplitude * sound.getSample(elapsed);
    }

    return sample;
}