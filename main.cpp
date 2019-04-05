#include "noiseInterface.h"

#include <thread>
#include <chrono>
#include <cmath>

static const float TWO_PI = 2 * M_PI;

void sleep(long long ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

float sinWave(float time) {
    return 0.5 * cos(440 * TWO_PI * time);
}

int main() {
    SoundMaker sound;

    sound.setSoundFunction(sinWave);
    sleep(2000);

    return 0;
}
