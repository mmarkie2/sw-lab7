//czasomierz.c
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

int stopCzasomierz = 0;
unsigned int czasomierzPosition = 0;
int stopCzasomierzSettings = 1;
unsigned int timeToCount = 0;
int isStarted = 0;
unsigned int startTime = 0;
unsigned int (*getTenthsOfSecond)();

void startCzasomierz(unsigned int (*_getTenthsOfSecond)()) {
    stopCzasomierz = 0;
    czasomierzPosition = 0;
    stopCzasomierzSettings = 0;
    getTenthsOfSecond = _getTenthsOfSecond;
    startTime = 0;
}

void loopCzasomierz() {
    if (stopCzasomierz == 0) {
        checkButtons();
        unsigned int tenthsOfSecond=0;
        if (isStarted == 1 && tenthsOfSecond<timeToCount ) {
            tenthsOfSecond = timeToCount -((*getTenthsOfSecond)() - startTime);
        }
        unsigned int tenthsOfSecond = (*getTenthsOfSecond)();
        int t = tenthsOfSecond % 10;
        int s = (tenthsOfSecond % (10 * 60)) - t;
        int m = (tenthsOfSecond % (10 * 60 * 60)) - t - s;
        int h = (tenthsOfSecond % (10 * 60 * 60 * 24)) - t - s - m;
        LCDClearScreen();
        LCDSetTime(h, m, s);
    }

}
unsigned int newTimeHour = 0;
unsigned int newTimeMinute = 0;
unsigned int newTimeSecond = 0;

void setNewTimeCandidate(int dt) {
    if (czasomierzPosition == 0) {
        newTimeHour += dt;
        if (newTimeHour > 23) {
            newTimeHour = 23;
        }
    } else if (czasomierzPosition == 1) {
        newTimeMinute += dt;
        if (newTimeMinute > 59) {
            newTimeMinute = 59;
        }
    } else if (czasomierzPosition == 2) {
        newTimeSecond += dt;
        if (newTimeSecond > 59) {
            newTimeSecond = 59;
        }
    }

    LCDSetTime(newTimeHour,newTimeMinute,newTimeSecond);
}

void checkButtons() {

    if (!(PIOB_PDSR & PIOB_SODR_P24)) // pb24 sw1
    {
        if (stopCzasomierzSettings == 0) {
            timeToCount=(((newTimeHour * 24 + newTimeMinute) * 60) + newTimeSecond) * 10;
            startTime = (*getTenthsOfSecond)();
            stopCzasomierzSettings = 1;
        } else {
            stopCzasomierzSettings = 0;

        }
    }
    if (stopCzasomierzSettings == 0) {
        if (!(PIOB_PDSR & PIOA_SODR_P9)) {//joystick left
            czasomierzPosition--;
            czasomierzPosition = czasomierzPosition % 3;
        } else if (!(PIOB_PDSR & PIOA_SODR_P14)) {//joystick right
            czasomierzPosition++;
            czasomierzPosition = czasomierzPosition % 3;
        } else if (!(PIOB_PDSR & PIOA_SODR_P7)) {//joystick up
            setNewTimeCandidate(1);
        } else if (!(PIOB_PDSR & PIOA_SODR_P15)) {//joystick down
            setNewTimeCandidate(-1);
        }
    }


}