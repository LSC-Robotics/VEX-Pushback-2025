#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "globals.h"

using namespace std;

void tempCheck() {
    while(true) {
        printf("Left Motor Group Temperature is: %.2f\n", mgL.temperature(celsius));
        printf("Right Motor Group Temperature is: %.2f\n", mgR.temperature(celsius));
        this_thread::sleep_for(1000);
        // This clears out the terminal using ANSI Escape Codes
        std::cout << "\033[2J\033[1;1H";
    }
}
