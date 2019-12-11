#ifndef __APPLICATION_H
#define __APPLICATION_H

#include <MDR32F9Qx_port.h>
#include <MDR32F9Qx_rst_clk.h>
#include "Pin.h"
#include "LcdKs0108.h"

#define SysTick_MAX 0xFFFFFF


class Application {
public:
    Application();

    void init();

    void iterate();

    void initSystick();

    void initHCSR04();

    void initGLcd();

    void systick();

    static void delayMks(uint32_t mks);

    static void delayMs(uint32_t ms);

    static uint16_t systickRounds;

private:
    LcdKs0108 *glcd;
    Pin *led;

    void ledInit();

    void displayInit();

    void initClk();

    void initIrq();
};


#endif /* __APPLICATION_H */