#pragma once

#include <vector>
#include <iostream>
#include <cmath>

#include "InstrumentSound.hpp"



class SoundInterface {

public:
    SoundInterface() {}

    float get_sample(double elapsed);

    std::vector<InstrumentSound> sounds;

private:
    float previous_sample = 0;
    float previous_elapsed = 0;
    int previous_size = 0;

};