#pragma once

#include <cmath>


class SoundInterface {

public:
    SoundInterface() {};

    float get_sample(float elapsed);

};