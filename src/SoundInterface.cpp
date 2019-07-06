#include "SoundInterface.hpp"


float SoundInterface::get_sample(float elapsed) {
    float amplitude = 0.5f;
    float frequency = 400.0f;
    return amplitude * sin(((double)frequency) * ((double)2 * 3.14159) * ((double)elapsed));
}