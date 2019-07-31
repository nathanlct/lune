#pragma once

#include <vector>
#include <cmath>

#include "InstrumentSound.hpp"



class SoundInterface {

public:
    SoundInterface() {}

    float get_sample(double elapsed);

    std::vector<InstrumentSound> sounds;

};