#include <iostream>
#include <cstdio>
#include <string>

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
    std::cout << "k, R(k)\n";
    for (int k = 0; k < size; k++) {
        float sum = 0;
        for (int n = 0; n < size - k - 1; n++) {
            sum += samples[n] * samples[n + k];
        }
        autocorrelation[k] = sum;
        std::cout << k << ", " << sum << '\n';
    }
    

}