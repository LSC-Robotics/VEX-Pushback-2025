#include "globals.h"
#include "vex.h"

brain br;

void colorCheck() {
    while(true) {
        vis1.takeSnapshot(redSig);
        if (vis1.objects[0].exists) {
            br.Screen.print("Red Object Found!");
        } else {
            br.Screen.print("Nothing found!");
        }
    }
}