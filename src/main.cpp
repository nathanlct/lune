#include <SFML/Window.hpp>

#include <thread>
#include <chrono>
#include <cmath>

#include "SoundIOBridge.hpp"
#include "InstrumentSound.hpp"
#include "SoundInterface.hpp"
#include "SineWaveInstrument.hpp"


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

    SineWaveInstrument sine;

    std::vector<float> frequencies;
    auto X = [](float n){ return pow(2, (n - 10) / 12) * 440; };

    sf::Window window(sf::VideoMode(800, 600), "Lune");
    window.setKeyRepeatEnabled(false);

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                {
                    float freq = 0;
                    switch(event.key.code) {
                        case sf::Keyboard::Q: freq = X(1); break;
                        case sf::Keyboard::Z: freq = X(2); break;
                        case sf::Keyboard::S: freq = X(3); break;
                        case sf::Keyboard::E: freq = X(4); break;
                        case sf::Keyboard::D: freq = X(5); break;
                        case sf::Keyboard::F: freq = X(6); break;
                        case sf::Keyboard::T: freq = X(7); break;
                        case sf::Keyboard::G: freq = X(8); break;
                        case sf::Keyboard::Y: freq = X(9); break;
                        case sf::Keyboard::H: freq = X(10); break;
                        case sf::Keyboard::U: freq = X(11); break;
                        case sf::Keyboard::J: freq = X(12); break;
                        case sf::Keyboard::K: freq = X(13); break;
                        case sf::Keyboard::O: freq = X(14); break;
                        case sf::Keyboard::L: freq = X(15); break;
                        case sf::Keyboard::P: freq = X(16); break;
                        case sf::Keyboard::M: freq = X(17); break;
                        default: break;
                    }
                    if(freq > 0)
                        sound_interface->sounds.push_back(sine.getSound(freq));
                    break;
                }
                default: break;
            }
        }
    }

    return 0;
}
