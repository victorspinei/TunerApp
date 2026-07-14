#ifndef AUDIO_HANDLER_H_
#define AUDIO_HANDLER_H_

#include <alsa/asoundlib.h>
#include <cstdlib>


class AudioHandler {
  public:
    AudioHandler() {};
    ~AudioHandler() { if (handle) snd_pcm_close(handle); };

    snd_pcm_t* get_handler() { return handle; };

    int open(const char *device) {
        int err = snd_pcm_open(&handle, device, SND_PCM_STREAM_CAPTURE, 0);
        if (err < 0) {
            std::cerr << snd_strerror(err) << '\n';
            return err;

        }
        return 0;
    };

    void configure(unsigned int rate, int channels) {
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
    };

    int read(float samples[], int frames) {
        float buffer[frames * 4];

        int err = snd_pcm_readi(handle, buffer, frames);
        if (err < 0) {
            std::cerr << snd_strerror(err) << '\n';
            return err;
        }

        for (int i = 0; i < frames; i++) {
            samples[i] = buffer[i * 4];
        }
        return 0;
    };

  private:
    snd_pcm_t *handle = nullptr;
    snd_pcm_hw_params_t *params = nullptr;

};

#endif