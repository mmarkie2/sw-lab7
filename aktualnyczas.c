//aktualnyczas.c
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

int stopAktualnyczas = 0;
unsigned int aktualnyPosition = 0;
int stopAktualnyczasSettings = 1;

unsigned int (*getTenthsOfSecond)();

void (*setTenthsOfSecond)(unsigned int);

void startAktualnyczas(unsigned int (*_getTenthsOfSecond)(), void (*_setTenthsOfSecond)(unsigned int)) {
    stopAktualnyczas = 0;
    aktualnyPosition = 0;
    stopAktualnyczasSettings = 1;
    getTenthsOfSecond = _getTenthsOfSecond;
    setTenthsOfSecond = _setTenthsOfSecond;

}

void loopAktualnyczas() {
    if (stopAktualnyczas == 0) {
        checkButtons();
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
    if (aktualnyPosition == 0) {
        newTimeHour += dt;
        if (newTimeHour > 23) {
            newTimeHour = 23;
        }
    } else if (aktualnyPosition == 1) {
        newTimeMinute += dt;
        if (newTimeMinute > 59) {
            newTimeMinute = 59;
        }
    } else if (aktualnyPosition == 2) {
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
        if (stopAktualnyczasSettings == 0) {
            stopAktualnyczasSettings = 1;
        } else {
            stopAktualnyczasSettings = 0;
            setTenthsOfSecond((((newTimeHour * 24 + newTimeMinute) * 60) + newTimeSecond) * 10);
        }
    }
    if (stopAktualnyczasSettings == 0) {
        if (!(PIOB_PDSR & PIOA_SODR_P9)) {//joystick left
            aktualnyPosition--;
            aktualnyPosition = aktualnyPosition % 3;
        } else if (!(PIOB_PDSR & PIOA_SODR_P14)) {//joystick right
            aktualnyPosition++;
            aktualnyPosition = aktualnyPosition % 3;
        } else if (!(PIOB_PDSR & PIOA_SODR_P7)) {//joystick up
            setNewTimeCandidate(1);
        } else if (!(PIOB_PDSR & PIOA_SODR_P15)) {//joystick down
            setNewTimeCandidate(-1);
        }
    }


}