#include "noiseInterface.h"
#include <SFML/Window.hpp>

#include <thread>
#include <chrono>
#include <cmath>

static const float TWO_PI = 2 * M_PI;

void sleep(long long ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

auto getSineWave(float max_amplitude) {
    return [=](float time, float frequency) {
        return max_amplitude * sin(frequency * TWO_PI * time);
    };
}

auto getSquareWave(float max_amplitude) {
    return [=](float time, float frequency) {
        float v = sin(frequency * TWO_PI * time);
        v = v > 0 ? 1 : -1;
        return max_amplitude * v;
    };
}

int main() {
    SoundMaker sound;

    // sound.setSoundFunction(getSquareWave(0.01));
    sound.setSoundFunction(getSineWave(0.3));

    while (true) {
        float n = 0;
        if      (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) n = 1;  // C
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) n = 2;  // C#
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) n = 3;  // D
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) n = 4;  // D#
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) n = 5;  // E
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) n = 6;  // F
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) n = 7;  // F#
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) n = 8;  // G
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) n = 9;  // G#
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) n = 10; // A
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) n = 11; // A#
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) n = 12; // B
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) n = 13; // C
        float f = (n == 0) ? 0 : pow(2, (n - 10) / 12) * 440;
        sound.setSoundFrequency(f);
        sleep(30);
    }

    return 0;
}
