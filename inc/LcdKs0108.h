//
// Created by Alexey Matukhin on 2019-04-02.
//

#ifndef R1_LCDKS0108_H
#define R1_LCDKS0108_H


#define CHIP1 0x01
#define CHIP2 0x02
#define DATA 0x80

#include <cstdint>
#include "Pin.h"

class LcdKs0108 {
public:
    void putPixel(uint8_t x, uint8_t y, bool bit);

    void waitForLcdReady(uint8_t state);

    uint8_t readByte(uint8_t state);

    void writeByte(uint8_t data, uint8_t state);

    void init(Pin *_DB0, Pin *_DB1, Pin *_DB2, Pin *_DB3, Pin *_DB4, Pin *_DB5, Pin *_DB6, Pin *_DB7, Pin *_E1,
              Pin *_E2, Pin *_RES,
              Pin *_RW, Pin *_CD, Pin *_E);

private:

     Pin * dataPins[8];
     Pin * E1;
     Pin * E2;
     Pin * RES;
     Pin * RW;
     Pin * CD;
     Pin * E;
};


#endif //R1_LCDKS0108_H
