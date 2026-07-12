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

        autocorrelation(correlation, samples, frames);

        auto period = findPeriod(correlation, frames, 0.3);

        if (period == 0) {
            continue; // no valid pitch detected
        }

        auto frequency = calculateFrequency(rate, period);

        int detected_index = get_detected_note(frequency);

        float cents = calculate_cents(
            frequency,
            note_frequencies[detected_index]
        );

        std::string status = get_status(cents);


        // update terminal
        std::cout << "\033[2J\033[H\n"; // clear terminal

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