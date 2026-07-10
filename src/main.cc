#include <iostream>
#include <alsa/asoundlib.h>

int main() {
    snd_pcm_t *handle;

    int err = snd_pcm_open(&handle, "hw:3,0",
                           SND_PCM_STREAM_CAPTURE, 0);

    if (err < 0) {
        std::cerr << snd_strerror(err) << '\n';
        return 1;
    }

    snd_pcm_hw_params_t *params;
    snd_pcm_hw_params_alloca(&params);

    snd_pcm_hw_params_any(handle, params);

    snd_pcm_hw_params_set_access(
        handle,
        params,
        SND_PCM_ACCESS_RW_INTERLEAVED);

    snd_pcm_hw_params_set_format(
        handle,
        params,
        SND_PCM_FORMAT_FLOAT_LE);

    snd_pcm_hw_params_set_channels(
        handle,
        params,
        4);

    unsigned int rate = 44100;
    snd_pcm_hw_params_set_rate_near(
        handle,
        params,
        &rate,
        nullptr);

    snd_pcm_hw_params(handle, params);

    snd_pcm_prepare(handle);

    const int frames = 2048;
    float buffer[frames * 4];

    err = snd_pcm_readi(handle, buffer, frames);

    if (err < 0) {
        std::cerr << snd_strerror(err) << '\n';
        return 1;
    }

    std::cout << "index, frame\n";
    float samples[frames];
    for (int i = 0; i < frames; i++) {
        samples[i] = buffer[i * 4];
        std::cout << i << ", " << samples[i] << '\n';
    }

    /*
    float min = samples[0];
    float max = samples[0];

    for (float s : samples) {
        if (s < min) min = s;
        if (s > max) max = s;
    }

    std::cout << "min = " << min
            << " max = " << max << '\n';
    */

    snd_pcm_close(handle);
}