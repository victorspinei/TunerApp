#ifndef NOTE_FREQUENCIES_H
#define NOTE_FREQUENCIES_H

#include <cstdio>
#include <cmath>
#include <string>
#include <vector>

const std::vector<std::string> note_names = {
    "A0", "A#0", "B0", "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A1", "A#1", "B1", "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A2", "A#2", "B2", "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A3", "A#3", "B3", "C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4", "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5", "A#5", "B5", "C6", "C#6", "D6", "D#6", "E6", "F6", "F#6", "G6", "G#6", "A6", "A#6", "B6", "C7", "C#7", "D7", "D#7", "E7", "F7", "F#7", "G7", "G#7", "A7", "A#7", "B7", "C8"
};
const int A4_index = 48; // A4

std::vector<float> note_frequencies;

std::string get_note_name(int semitones) {
    if (A4_index + semitones >= note_names.size() || A4_index + semitones < 0) {
        std::cerr << "Note out of bounds\n";
        exit(1);
    }
    return note_names[A4_index + semitones];
}

float note_frequency(float reference_frequency, int semitones) {
    float frequency = reference_frequency * std::pow(2, float(semitones) / 12);
    return frequency;
}

void initialize_notes(float reference_frequency) {
    for (int i = 0; i < note_names.size(); i++) {
        note_frequencies.push_back(note_frequency(reference_frequency, i - A4_index));
    }
}

int get_detected_note(float frequency) {
    auto it = lower_bound(note_frequencies.begin(), note_frequencies.end(), frequency);
    int index = it - note_frequencies.begin();

    int target;

    if (index == 0)
        target = 0;
    else if (index == note_frequencies.size())
        target = note_frequencies.size() - 1;
    else {
        float lowerDiff = frequency - note_frequencies[index - 1];
        float upperDiff = note_frequencies[index] - frequency;

        target = (lowerDiff < upperDiff) ? index - 1 : index;
    }

    return target;
}

float calculate_cents(float frequency, float target_frequency) {
    float cents = 1200.0f * std::log2(frequency / target_frequency); 
    return cents;
}

std::string get_status(float cents) {
    if (std::abs(cents) <= 2.0) 
        return "In tune";
    else if (std::abs(cents) <= 10.0) 
        return "Slightly off";
    else 
        return "Tune up/down";
}

#endif