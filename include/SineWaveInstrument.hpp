#pragma once

#include "InstrumentSound.hpp"

class SineWaveInstrument {

public:
    SineWaveInstrument() {}

    InstrumentSound getSound(float frequency) {
        InstrumentSound sound;
        sound.sampleFunction = [=](double elapsed) { return 0.3*sin(2 * M_PI * frequency * elapsed); };
        return sound;
    }

};