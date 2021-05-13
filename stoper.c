#include <LCD.h>
#include <cstdio>
#include "stdlib.h"

void checkButtons();

void LCDSetTime(int h, int m, int s) {
    char *outstr = (char *) malloc(20);
    sprintf(outstr,
            "%d:%d:%d ", h, m, s);

    LCDPutStr(outstr,
              60, 50, LARGE, WHITE, BLACK);
    free(outstr);

}


unsigned int (*getTenthsOfSecond)();

int isStarted = 0;
int stopStoper = 1;
unsigned int startTime;

void startStoper(unsigned int (*_getTenthsOfSecond)()) {
    stopStoper = 0;

    getTenthsOfSecond = _getTenthsOfSecond;
    startTime = 0;

}

void loopStoper() {
    if (stopStoper == 0) {
        checkButtons();
        unsigned int tenthsOfSecond=0;
        if (isStarted == 1) {
            tenthsOfSecond = (*getTenthsOfSecond)() - startTime;
        }

        int t = tenthsOfSecond % 10;
        int s = (tenthsOfSecond % (10 * 60)) - t;
        int m = (tenthsOfSecond % (10 * 60 * 60)) - t - s;
        int h = (tenthsOfSecond % (10 * 60 * 60 * 24)) - t - s - m;
        LCDClearScreen();
        LCDSetTime(h, m, s);
    }


}


void checkButtons() {

    if (!(PIOB_PDSR & PIOB_SODR_P24)) // pb24 sw1
    {
        if (isStarted == 0) {
            isStarted = 1;
            startTime = (*getTenthsOfSecond)();
        } else {
            isStarted = 0;
        }
    }


}