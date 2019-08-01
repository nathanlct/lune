#pragma once


#include <functional>
#include <iostream>

class InstrumentSound {
public:
    InstrumentSound();

    float getRawSample(double elapsed);
    float getAmplitude(double elapsed);
    bool isFinished() const;

public:
    std::function<float(double)> sampleFunction;

private:
    float attackTime = 0.05;
    float decayTime = 0.05;
    float sustainTime = 2.0;
    float releaseTime = 0.075;
    float attackAmplitude = 0.5f;
    float sustainAmplitude = 0.1f;

    double startingTime = -1;

    bool finished = false;
};

