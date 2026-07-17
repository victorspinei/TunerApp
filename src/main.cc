#include <iostream>
#include <algorithm>

#include "audio_handler.h"
#include "signal_processor.h"
#include "tuner.h"
#include "app.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    const int frames = 8192;
    const unsigned int rate = 48000;

    AudioHandler handler = AudioHandler();

    int err = handler.open("hw:3,0");
    if (err < 0) {
        std::cerr << "No device\n";
    }

    handler.configure(rate, 4);

    SignalProcessor processor = SignalProcessor(frames, rate);

    float reference_frequency = 440.0;
    Tuner tuner = Tuner(reference_frequency);

    Window window(handler, processor, tuner, frames, reference_frequency);

    window.show();

    return app.exec();
}