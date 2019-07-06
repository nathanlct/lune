#include <SFML/Window.hpp>

#include <thread>
#include <chrono>
#include <cmath>

#include "SoundIOBridge.hpp"
#include "SoundInterface.hpp"


void sleep(long long ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

std::function<double(float,float)> getBasicWave(int type, float max_amplitude = 1) {
    return [=](float elapsed_time, float frequency) -> float {
        switch (type) {
            case 0: // Sine wave
                return .3 * max_amplitude * sin(((double)frequency) * ((double)2 * 3.14159) * ((double)elapsed_time));
            case 1: // Square wave
                return .01 * max_amplitude * (sin(frequency * 2 * 3.14159 * elapsed_time) > 0 ? 1 : -1);
            case 2: // Triangle wave
                return .3 * (2 * max_amplitude / 3.14159) * (asin(sin(frequency * 3.14159 * elapsed_time)));
                // + asin(sin(440 * TWO_PI * elapsed_time)));
            case 3: // Raw saw wave
                return .05 * (2 * max_amplitude / 3.14159) * (frequency * 3.14159 * fmod(elapsed_time, 1.0 / frequency) - 3.14159 / 2.0);
            case 4: // Soft saw wave (higher precision = close to raw saw wave, lower precision = softer)
            {
                float precision = 30.0;
                float v = 0;
                int sign = -1;
                for (float n = 1.0; n < 30.0; n++, sign *= -1)
                    v += sign * (sin(n * frequency * 2 * 3.14159 * elapsed_time)) / n;                
                return .1 * (max_amplitude / 2 - max_amplitude * v / 3.14159);
            }
            case 5: // Random noise
                return (frequency == 0) ? 0 : .2 * 2.0 * ((double)rand() / (double)RAND_MAX) - 1.0;
            default:
                return 0;
        }

    };
}

int main() {
    SoundInterface* sound_interface = new SoundInterface();
    SoundIOBridge sound_io_bridge(sound_interface);

    sleep(5000);
    

    // sound.setSoundFunction(getSquareWave(0.01));
    // sound.setSoundFunction(getBasicWave(0));


    // float f = 0;
    // while (true) {
    //     float m = 1;
    //     float f = 0;
    //     auto X = [](float n){return pow(2, (n - 10) / 12) * 440;};
    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) f += X(1);  // C
    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) f += X(2);  // C#
    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) f += X(3);  // D
    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) f += X(4);  // D#
    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) f += X(5);  // E
    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) f += X(6);  // F
    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) f += X(7);  // F#
    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) f += X(8);  // G
    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) f += X(9);  // G#
    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) f += X(10); // A
    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) f += X(11); // A#
    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) f += X(12); // B
    //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) f += X(13); // C
    //     else m = 0;

    //     sound.setSoundFrequency(f);
    //     sleep(30);
    // }

    return 0;
}
