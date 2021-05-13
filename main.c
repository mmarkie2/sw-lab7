//main.c

#include <LCD.h>
#include <targets/AT91SAM7.h>
#include "menu.c"

int main() {

    InitLCD();
    LCDSettings();
    LCDClearScreen();
    menuInit();
    printMenu();
    while (1) {
        loopMenu();
    }
}
