#include <iostream>
#include <algorithm>

#include "audio_handler.h"
#include "signal_processor.h"
#include "tuner.h"
#include "app.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Window window;
    window.show();

    const int frames = 8192;
    unsigned int rate = 48000;
    SignalProcessor signalProcessor = SignalProcessor(frames, rate);

    AudioHandler handler = AudioHandler();

    float reference_frequency = 440.0;
    Tuner tuner = Tuner(reference_frequency);

    int err = handler.open("hw:3,0");
    if (err < 0) {
        window.setStatus("Audio device not connected!");
    } else {
        window.setStatus("Ready");
    }

    float samples[frames];

    handler.configure(rate, 4);

    // handler.read(frames, samples);
    // float frequency = pitchDetector.detect(samples);
    // if (frequency = 0) "Pluck a string"

    return app.exec();
}