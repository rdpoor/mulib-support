#include "morse_2.h"
#include <unistd.h>

/**
 * main.c
 */

int main(int argc, char *argv[])
{
    int verbosityLevel = 0;
    int opt;
    while ((opt = getopt(argc, argv, "vV")) != -1) {
        switch (opt) {
            case 'v': verbosityLevel = 1; break;
            case 'V': verbosityLevel = 2; break;
        }
    }
    morse_2_init(verbosityLevel); // this will hang until a key press or button press happens
    while(1) {
        morse_2_step();
    }
}
