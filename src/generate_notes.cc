#include <iostream>
#include <string>
#include <vector>

int main() {
    int index = 0;
    std::vector<std::string> notes = 
        {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"}; 

    while (index < 8) {
        for (int i = 0; i < 12; i++) {
            if (notes[i] == "C") index++;
            if (index == 8) break;
            std::cout << '\"' << notes[i] << index << "\", ";
        }
    }

    std::cout << "\"C8\"" << std::endl;
}