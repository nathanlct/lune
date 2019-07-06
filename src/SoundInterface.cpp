#include "SoundInterface.hpp"


float SoundInterface::get_sample(float elapsed) {
    float amplitude = 0.3f;
    float frequency = 400.0f;
    return amplitude * sin(2 * M_PI * frequency * elapsed);
}