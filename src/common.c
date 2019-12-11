//
// Created by maoz on 03.09.2018.
//

#include <stdint.h>
#include "common.h"


#ifdef __cplusplus
extern "C" {
#endif

void delay(int waitTicks) {
    int i;
    for (i = 0; i < waitTicks; i++) {
        asm("nop");
    }
}

void delayMicroseconds(uint32_t delay) {
    delay *= 40;
    while (delay > 0) {
        delay--;
    }
}

#ifdef __cplusplus
} // extern "C" block end
#endif