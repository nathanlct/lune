#include "InstrumentSound.hpp"



InstrumentSound::InstrumentSound() {}


float InstrumentSound::getRawSample(double elapsed) {
    return sampleFunction(elapsed);
}

float InstrumentSound::getAmplitude(double elapsed) {
    if(startingTime == -1) {
        startingTime = elapsed;
    }
    float timeSinceStart = elapsed - startingTime;

    float amplitude = 0.0f;
    if(timeSinceStart <= attackTime) 
        amplitude = attackAmplitude * timeSinceStart / attackTime;
    else if(timeSinceStart <= attackTime + decayTime)
        amplitude = attackAmplitude - (attackAmplitude - sustainAmplitude) * (timeSinceStart - attackTime) / decayTime;
    else if(timeSinceStart <= attackTime + decayTime + sustainTime)
        amplitude = sustainAmplitude;
    else if(timeSinceStart <= attackTime + decayTime + sustainTime + releaseTime)
        amplitude = sustainAmplitude * (1.0f - (timeSinceStart - attackTime - decayTime - sustainTime) / releaseTime);
    else {
        amplitude = 0.0f;
        finished = true;
    }

    return amplitude;
}

bool InstrumentSound::isFinished() const {
    return finished;
}