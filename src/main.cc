#include <iostream>
#include "audio_samples.h"
#include "calculations.h"

int main() {
    unsigned int rate = 44100;
    int channels = 4;

    auto handle = init("hw:3,0", rate, channels);
    
    const int frames = 2048;
    float samples[frames];
    float correlation[frames];

    read_samples(handle, frames, samples);
    autocorrelation(correlation, samples, frames);

    auto period = findPeriod(correlation, frames, 0.3);
    auto frequency = calculateFrequency(rate, period);

    std::cout << "Frequency: " << frequency << " Hz\n";

    close_pcm(handle);
}