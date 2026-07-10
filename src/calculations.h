#ifndef CALCULATIONS_H 
#define CALCULATIONS_H 

#include <string>
#include <vector>

int findPeriod(float arr[], int n, float threshold_value){
    float threshold = threshold_value * arr[0];

    for (int k = 120; k < 650; k++) {
        if (arr[k] > arr[k-1] and
            arr[k] > arr[k+1] and
            arr[k] > threshold) {
            return k;
        }
    }

    return -1;
}

void autocorrelation(float correlation[], float samples[], int size) {
    float energy = 0;

    for(int i = 0; i < size; i++)
        energy += samples[i] * samples[i];


    for(int k = 0; k < size; k++) {

        float sum = 0;

        for(int n = 0; n < size-k; n++)
            sum += samples[n] * samples[n+k];

        correlation[k] = sum / energy;
    }
}

float calculateFrequency(int rate, float period) {
    return (float)rate / period;
}

#endif