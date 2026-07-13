#ifndef CALCULATIONS_H 
#define CALCULATIONS_H 

#include <string>
#include <vector>
#include <cmath>

float findPeriod(float arr[], int size, int rate, float threshold_value) {
    int minLag = rate / 2000;
    int maxLag = rate / 40;

    float threshold = threshold_value;
    float bestValue = threshold;

    int peak = -1;

    for (int k = minLag + 1; k < maxLag - 1; k++)
    {
        if (arr[k] > arr[k-1] &&
            arr[k] > arr[k+1] &&
            arr[k] > bestValue)
        {
            bestValue = arr[k];
            peak = k;
        }
    }

    if (peak == -1)
        return -1.0;

    float left = arr[peak - 1];
    float center = arr[peak];
    float right = arr[peak + 1];

    float denominator = left - 2.0f * center + right;

    if (fabs(denominator) < 0.0001f)
        return peak;

    float offset = 0.5f * (left - right) / denominator;

    return peak + offset;
}

void autocorrelation(float correlation[], float samples[], int size) {
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

float calculateFrequency(int rate, float period) {
    return (float)rate / period;
}

void removeDC(float samples[], int size) {
    float mean = 0.0;
    for (int i = 0; i < size; i++)
        mean += samples[i];
    mean /= size;

    for (int i = 0; i < size; i++)
        samples[i] -= mean;
}

void HannWindow(float samples[], int size) {
    for (int i = 0; i < size; i++) {
        float w = 0.5f * (1.0f - cosf(2.0f * M_PI * i / (size - 1)));
        samples[i] *= w;
    }
}

float getRMS(float samples[], int size) {
    float sum = 0;

    for(int i = 0; i < size; i++)
        sum += samples[i] * samples[i];

    return sqrt(sum / size);
}

float smoothFrequency(float frequency) {
    const int size = 5;
    static float history[size] = {};
    static int index = 0;

    history[index] = frequency;
    index = (index + 1) % size;

    float sum = 0;

    for (int i = 0; i < size; i++)
        sum += history[i];

    return sum / size;
}

#endif