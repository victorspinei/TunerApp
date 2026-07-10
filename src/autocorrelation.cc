#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

int findPeriod(float arr[], int n){
    float threshold = 0.3 * arr[0];

    for (int k = 100; k < 600; k++) {
        if (arr[k] > arr[k-1] and
            arr[k] > arr[k+1] and
            arr[k] > threshold) {
            return k;
        }
    }

    return -1;
}

int main() {
    std::freopen("test", "r", stdin);

    int size = 2048;
    std::string inp;
    float samples[size];
    float autocorrelation[size];

    for (int i = 0; i < size; i++) {
        std::cin >> i;
        std::cin >> inp;
        samples[i] = std::stof(inp);
    }
    //std::cout << "k, R(k)\n";
    for (int k = 0; k < size; k++) {
        float sum = 0;
        for (int n = 0; n < size - k - 1; n++) {
            sum += samples[n] * samples[n + k];
        }
        autocorrelation[k] = sum;
        //std::cout << k << ", " << sum << '\n';
    }
    
    //std::cout << findPeriod(autocorrelation, size) << '\n';
    auto period = findPeriod(autocorrelation, size);
    int rate = 44100;
    auto frequency = (float)rate / period;
    std::cout << "Frequency: " << frequency << " Hz\n";
}