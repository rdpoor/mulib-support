#include "morse_3.h"
#include <unistd.h>


/**
 * main.c
 */

int main(int argc, char *argv[])
{
    int aVerbosityLevel = 0;
    int opt;
    while ((opt = getopt(argc, argv, "vV")) != -1) {
        switch (opt) {
            case 'v': aVerbosityLevel = 1; break;
            case 'V': aVerbosityLevel = 2; break;
        }
    }
    morse_3_init(aVerbosityLevel); // this will hang until a key press ir button press appens
    while(1) {
        morse_3_step();
    }
}
