#include <iostream>
#include <algorithm>

#include "audio_samples.h"
#include "calculations.h"
#include "note_frequencies.h"

int main() {
    float reference_frequency = 440.00; // Hz
    initialize_notes(reference_frequency);

    unsigned int rate = 48000;
    int channels = 4;

    auto handle = init("hw:3,0", rate, channels);
    
    const int frames = 8192;
    float samples[frames];
    float correlation[frames];

    while (true) {
        read_samples(handle, frames, samples);

        removeDC(samples, frames);

        if (getRMS(samples, frames) < 0.0001) {
            continue;
        }

        HannWindow(samples, frames);

        autocorrelation(correlation, samples, frames);

        auto period = findPeriod(correlation, frames, rate, 0.7);

        if (period <= 0) {
            continue; 
        }

        auto frequency = calculateFrequency(rate, period);
        frequency = smoothFrequency(frequency);

        int detected_index = get_detected_note(frequency);

        float cents = calculate_cents(
            frequency,
            note_frequencies[detected_index]
        );

        std::string status = get_status(cents);


        std::cout << "\033[2J\033[H\n"; 

        std::cout << "Detected: "
                  << note_names[detected_index] << '\n';

        std::cout << "Frequency: "
                  << frequency << " Hz\n";

        std::cout << "Target: "
                  << note_frequencies[detected_index] << " Hz\n";

        std::cout << "Offset: "
                  << (cents > 0 ? "+" : "")
                  << cents << " cents\n";

        std::cout << "Status: "
                  << status << '\n';
    }

    close_pcm(handle);
}