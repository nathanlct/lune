#pragma once

#include "InstrumentSound.hpp"

class SineWaveInstrument {

public:
    SineWaveInstrument() {}

    InstrumentSound getSound(float frequency) {
        InstrumentSound sound;
        sound.sampleFunction = [=](double elapsed) { return sin(2 * M_PI * frequency * elapsed); };
        return sound;
    }

};