#include "SoundInterface.hpp"



float SoundInterface::get_sample(double elapsed) {
    float sample = 0.0f;
    float amplitude = 0.0f;
    for(auto& sound: sounds) {
        sample += sound.getAmplitude(elapsed) * sound.getRawSample(elapsed);
        amplitude += sound.getAmplitude(elapsed);
    }
    // if(amplitude > 0 && sounds.size() > 1) {
    //     sample /= amplitude;
    // }
    // if(sounds.size() > 0)
    //     sample /= sounds.size();

    float diff = abs(sample - previous_sample);
    previous_sample = sample;

    if(diff > 0.1) {
        std::cout << "\nsample diff: " << diff << "\n";
        // for(auto& sound: sounds) {
        //     // std::cout << "\t sound now " << sound.getSample(elapsed) << " - previous " << sound.getSample(previous_elapsed) << "\n";
        // }
        std::cout << "elapsed diff: " << abs(previous_elapsed - elapsed) << "\n";
        std::cout << "size: " << sounds.size() << " - previous " << previous_size << "\n";
    }

    previous_size = sounds.size();
    previous_elapsed = elapsed;

    // todo not efficient
    sounds.erase(std::remove_if(sounds.begin(), sounds.end(), [](InstrumentSound& s) { return s.isFinished(); }), sounds.end());

    // std::cout << sounds.size() << std::endl;
    // if(sounds.size() > 0) if(sounds[0].isFinished()) std::cout << "a\n";

    return sample;
}