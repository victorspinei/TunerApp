#ifndef AUDIO_SAMPLES_H
#define AUDIO_SAMPLES_H

#include <alsa/asoundlib.h>
#include <cstdlib>

snd_pcm_t *init(const char *device, unsigned int rate , int channels) {
    snd_pcm_t *handle;

    int err = snd_pcm_open(&handle, device,
                           SND_PCM_STREAM_CAPTURE, 0);

    if (err < 0) {
        std::cerr << snd_strerror(err) << '\n';
        std::exit(1);
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
        channels);

    snd_pcm_hw_params_set_rate_near(
        handle,
        params,
        &rate,
        nullptr);

    snd_pcm_hw_params(handle, params);

    snd_pcm_prepare(handle);

    return handle;
}

void close_pcm(snd_pcm_t *handle) { 
    snd_pcm_close(handle);
}

void read_samples(snd_pcm_t *handle, int frames, float samples[]) {
    float buffer[frames * 4];

    int err = snd_pcm_readi(handle, buffer, frames);

    if (err < 0) {
        std::cerr << snd_strerror(err) << '\n';
        std::exit(1);
    }

    for (int i = 0; i < frames; i++) {
        samples[i] = buffer[i * 4];
    }
}

#endif