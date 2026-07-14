#ifndef SIGNAL_PROCESSOR_H
#define SIGNAL_PROCESSOR_H

#include <cstdio>
#include <cmath>
#include <string>
#include <vector>

class SignalProcessor {
  public:
    SignalProcessor(int size, int rate): size(size), rate(rate), correlation(size), history(5, 0.0f) {}

    float findPeriod(float threshold_value) {
        int minLag = rate / 2000;
        int maxLag = rate / 40;

        float bestValue = threshold_value;
        int peak = -1;

        for (int k = minLag + 1; k < maxLag - 1; k++)
        {
            if (correlation[k] > correlation[k-1] &&
                correlation[k] > correlation[k+1] &&
                correlation[k] > bestValue)
            {
                bestValue = correlation[k];
                peak = k;
            }
        }

        if (peak == -1)
            return -1.0;

        float left = correlation[peak - 1];
        float center = correlation[peak];
        float right = correlation[peak + 1];

        float denominator = left - 2.0f * center + right;

        if (fabs(denominator) < 0.0001f)
            return peak;

        float offset = 0.5f * (left - right) / denominator;

        return peak + offset;
    }

    void autocorrelation(float samples[]) {
        for (int k = 0; k < size; k++) {

            float sum = 0;
            float energy1 = 0;
            float energy2 = 0;

            for (int n = 0; n < size - k; n++)
            {
                sum += samples[n] * samples[n + k];

                energy1 += samples[n] * samples[n];
                energy2 += samples[n + k] * samples[n + k];
            }

            correlation[k] = sum / sqrtf(energy1 * energy2);
        }
    }

    void removeDC(float samples[]) {
        float mean = 0.0;
        for (int i = 0; i < size; i++)
            mean += samples[i];
        mean /= size;

        for (int i = 0; i < size; i++)
            samples[i] -= mean;
    }

    float calculateFrequency(float period) {
        return (float)rate / period;
    }

    float getRMS(float samples[]) {
        float sum = 0;

        for(int i = 0; i < size; i++)
            sum += samples[i] * samples[i];

        return sqrt(sum / size);
    }

    void HannWindow(float samples[]) {
        for (int i = 0; i < size; i++) {
            float w = 0.5f * (1.0f - cosf(2.0f * M_PI * i / (size - 1)));
            samples[i] *= w;
        }
    }

    float smoothFrequency(float frequency) {
        history[index] = frequency;
        index = (index + 1) % history.size();

        float sum = 0;

        for (int i = 0; i < history.size(); i++)
            sum += history[i];

        return sum / size;
    }

    float detect(float samples[]) {
        removeDC(samples);

        if(getRMS(samples) < 0.0001)
            return 0;


        HannWindow(samples);

        autocorrelation(samples);

        float period = findPeriod(0.7);

        if(period <= 0)
            return 0;

        float frequency = calculateFrequency(period);
        //frequency = smoothFrequency(frequency);

        return frequency;
    }

  private:
    int size;
    int rate;
    std::vector<float> correlation;
    std::vector<float> history;
    int index = 0;
};

#endif