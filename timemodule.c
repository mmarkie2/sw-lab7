//timemodule.c
#include <targets/AT91SAM7.h>


unsigned int tenthsOfSecond = 0;
unsigned int tenthsOfSecondMax = 10 * 60 * 60 * 24;

unsigned int getCurrentTenthsOfSecond() {
    return tenthsOfSecond;
}

void setCurrentTenthsOfSecond(unsigned int time) {
    tenthsOfSecond = time;
}

void interruptHandlerTime() {
    if (PIT_SR_PITS) {
        tenthOfSecond++;
      if(tenthsOfSecond>tenthsOfSecondMax)
      {
          tenthOfSecond=0;
      }
    }
    AIC_EOICR = 1;
}

void initClockInterrupt() {

    AIC_IDCR = AIC_IDCR_SYS;
    AIC_SVR1 = (unsigned long) interruptHandlerTime;
    AIC_SMR1 |= (1 << 5);

    AIC_SMR1 |= 4;
    AIC_ICCR = AIC_ICCR_SYS;

    PIT_MR = (62500 & PIT_MR_PIV_MASK); // interrupt every 1/10 of second
    PIT_MR |= PIT_MR_PITEN;//turn on pit timer
    PIT_MR |= PIT_MR_PITIEN;//turn off interrupts on pit
    PIT_PIVR;//reading PIT_PIVR clears PITS in PIT_SR

    __asm__("MRS r1, CPSR");
    __asm__("BIC r1, r1, #0x80");
    __asm__("MSR CPSR, r1");
}


