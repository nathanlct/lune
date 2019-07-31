#include "InstrumentSound.hpp"



InstrumentSound::InstrumentSound() {}


float InstrumentSound::getSample(double elapsed) {
    if(startingTime == -1) {
        startingTime = elapsed;
    }
    float timeSinceStart = elapsed - startingTime;

    float rawSample = sampleFunction(elapsed);

    float amplitude = 0.0f;
    if(timeSinceStart <= attackTime) 
        amplitude = attackAmplitude * timeSinceStart / attackTime;
    else if(timeSinceStart <= attackTime + decayTime)
        amplitude = attackAmplitude - (attackAmplitude - sustainAmplitude) * (timeSinceStart - attackTime) / decayTime;
    else if(timeSinceStart <= attackTime + decayTime + sustainTime)
        amplitude = sustainAmplitude;
    else if(timeSinceStart <= attackTime + decayTime + sustainTime + releaseTime)
        amplitude = sustainAmplitude * (1.0f - (timeSinceStart - attackTime - decayTime - sustainTime) / releaseTime);
    else
        amplitude = 0.0f;

    return amplitude * rawSample;
}