//menu.c

#include <LCD.h>
#include <obraz.c>
#include <gra.c>
#include <potencjometr.c>
#include <temperatura.c>

#include <info.c>
#include "timemodule.c"
#include "aktualnyczas.c"
#include "stoper.c"
#include "czasomierz.c"

#define PUSHPIN PIOA_SODR_P8

#define UPPIN PIOA_SODR_P7
#define DOWNPIN PIOA_SODR_P15
#define LEFTPIN PIOA_SODR_P9
#define RIGHTPIN PIOA_SODR_P14

int isProgramRunning = 0;
int menuPosition = 0;
int menuLevel = 0;

int arr0size = 3;
char menu0str[3][20] =
        {"PROGRAMY",
         "ZEGAR",
         "INFO O AUTORZE",
        };
int arr1size = 4;
char menu1str[4][20] =
        {"OBRAZ",
         "GRA",
         "POTENCJOMETR",
         "TEMPERATURA",
        };
int arr2size = 3;
char menu2str[3][20] =
        {"AKTUALNY CZAS",
         " STOPER",
         "  CZASOMIERZ"

        };

int arr3size = 1;
char menu3str[1][20] =
        {"INFO O AUTORZE"

        };

void menuInit() {

    PMC_PCER = PMC_PCER_PIOB;
    PMC_PCER = PMC_PCER_PIOA;
    //push
    PIOA_ODR |= PUSHPIN; //set as input
    PIOA_PER |= PUSHPIN; //turn on pin
    PIOA_PUER = PUSHPIN; //enable pull-up

    //up
    PIOA_ODR |= UPPIN; //set as input
    PIOA_PER |= UPPIN; //turn on pin
    PIOA_PUER = UPPIN; //enable pull-up

    //down
    PIOA_ODR |= DOWNPIN; //set as input
    PIOA_PER |= DOWNPIN; //turn on pin
    PIOA_PUER = DOWNPIN; //enable pull-up

    //left
    PIOA_ODR |= LEFTPIN; //set as input
    PIOA_PER |= LEFTPIN; //turn on pin
    PIOA_PUER = LEFTPIN; //enable pull-up

    //right
    PIOA_ODR |= RIGHTPIN; //set as input
    PIOA_PER |= RIGHTPIN; //turn on pin
    PIOA_PUER = RIGHTPIN; //enable pull-up

    //SW2
    PIOA_ODR |=PIOB_SODR_P25; //set as input
    PIOA_PER |= PIOB_SODR_P25; //turn on pin
    PIOA_PUER = PIOB_SODR_P25; //enable pull-up

    //SW1
    PIOA_ODR |= PIOB_SODR_P24; //set as input
    PIOA_PER |= PIOB_SODR_P24; //turn on pin
    PIOA_PUER = PIOB_SODR_P24; //enable pull-up

    PIOB_OER = PIOB_SODR_P19;//buzzer init
    PIOB_PER = PIOB_SODR_P19;
    //initializing timer
    initClockInterrupt();

}

void printMenu();

void onUpMenu() {
    menuPosition--;
    if (menuPosition < 0) {
        menuPosition = 0;
    }
    printMenu();
}

void onDownMenu() {

    int arrSize = 0;
    if (menuLevel == 0) {
        arrSize = arr0size;

    } else if (menuLevel == 1) {
        arrSize = arr1size;

    } else if (menuLevel == 2) {
        arrSize = arr2size;

    }
    menuPosition++;
    if (menuPosition >= arrSize) {
        menuPosition = arrSize - 1;
    }
    printMenu();
}

void onEnterMenu() {
    if (menuLevel == 0) {
        if (menuPosition == 0) {
            menuLevel = 1;
            menuPosition = 0;
        } else if (menuPosition == 1) {
            menuLevel = 2;
            menuPosition = 0;
        } else if (menuPosition == 2) {
            menuLevel = 3;
            menuPosition = 0;
        }
    } else if (menuLevel == 1) {
        isProgramRunning = 1;
        if (menuPosition == 0) {
            menuLevel = 11;
            menuPosition = 0;
            startObraz();
        } else if (menuPosition == 1) {
            menuLevel = 12;
            menuPosition = 0;
            startGra();
        } else if (menuPosition == 2) {
            menuLevel = 13;
            menuPosition = 0;
            startPotencjometr();
        } else if (menuPosition == 3) {
            menuLevel = 14;
            menuPosition = 0;
            startTemperatura();
        }
    } else if (menuLevel == 2) {
        isProgramRunning = 1;
        if (menuPosition == 0) {
            menuLevel = 21;
            menuPosition = 0;
            startAktualnyczas();
        } else if (menuPosition == 1) {
            menuLevel = 22;
            menuPosition = 0;
            startStoper();
        } else if (menuPosition == 2) {
            menuLevel = 23;
            menuPosition = 0;
            startCzasomierz();
        }
    } else if (menuLevel == 3) {
        isProgramRunning = 1;
        if (menuPosition == 0) {
            menuLevel = 31;
            menuPosition = 0;
            startInfo();
        }

    }

    printMenu();
}

void onEscapeMenu() {
    isProgramRunning = 0;
    if (menuLevel == 0) {
        //do nothing
    } else if (menuLevel == 1 || menuLevel == 2 || menuLevel == 3) {
        menuLevel = 0;
    } else if (10 < menuLevel && menuLevel < 20) {
        stopGra = 1;
        stopPotecjometr = 1;
        stopTemperatura = 1;
        menuLevel = 1;
    } else if (20 < menuLevel && menuLevel < 30) {
        stopAktualnyczas = 1;
        stopAktualnyczasSettings = 1;
        stopStoper = 1;
        stopCzasomierz=1;
        menuLevel = 2;
    } else if (30 < menuLevel && menuLevel < 40) {
        menuLevel = 3;

    }
    printMenu();
}


void printMenu() {
    LCDClearScreen();
    int arrSize = 0;
    char *menuStr;
    if (menuLevel == 0) {
        arrSize = arr0size;
        menuStr = menu0str;
    } else if (menuLevel == 1) {
        arrSize = arr1size;
        menuStr = menu1str;
    } else if (menuLevel == 2) {
        arrSize = arr2size;
        menuStr = menu2str;
    } else if (menuLevel == 3) {
        arrSize = arr3size;
        menuStr = menu3str;
    } else {
        return;
    }

    int i = 0;
    for (
            i = 0;
            i < arrSize;
            ++i) {
        if (i == menuPosition) {//current position in different color
            LCDPutStr(menuStr[i],
                      0, i * 10, MEDIUM, WHITE, RED);
        } else {
            LCDPutStr(menuStr[i],
                      0, i * 10, MEDIUM, WHITE, BLACK);
        }

    }

}

void buzz() {
    int j = 0;
    for (j = 0; j < 20; j++) {
        PIOB_SODR |= PIOB_SODR_P19;//pb19 buzzer

        int i = 0;
        for (i = 0; i < 400; i++) {
            __asm__("nop");
        }
        PIOB_CODR |= PIOB_SODR_P19;

        for (i = 0; i < 400; i++) {
            __asm__("nop");
        }
    }

}

void checkButtonsMenu() {
    if (isProgramRunning == 0) {
        if (!(PIOB_PDSR & PIOB_SODR_P24)) // pb24 sw1
        {
            onEnterMenu();
            buzz();
        } else if (!(PIOA_PDSR & PIOA_SODR_P14)) // pa14 joystick down
        {
            onDownMenu();
            buzz();
        } else if (!(PIOA_PDSR & PIOA_SODR_P7)) // pa7 joystick up
        {
            onUpMenu();
            buzz();
        }
    }
    //escape always available

    if (!(PIOB_PDSR & PIOB_SODR_P25)) // pb25 sw2
    {

        onEscapeMenu();
        buzz();
    }
}

void loopMenu() {
    checkButtonsMenu();
    loopGra();
    loopTemperatura();
    loopAktualnyczas();
    loopStoper();
    loopCzasomierz();

}