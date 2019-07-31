#pragma once


#include <functional>

class InstrumentSound {
public:
    InstrumentSound();

    float getSample(double elapsed);

public:
    std::function<float(double)> sampleFunction;

private:
    float attackTime = 0.1;
    float decayTime = 0.1;
    float sustainTime = 1.0;
    float releaseTime = 0.25;
    float attackAmplitude = 0.5f;
    float sustainAmplitude = 0.3f;

    double startingTime = -1;
};

