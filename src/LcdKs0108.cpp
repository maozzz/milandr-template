//
// Created by Alexey Matukhin on 2019-04-02.
//

#include <common.h>
#include <Pin.h>
#include <Application.h>
#include "LcdKs0108.h"

uint8_t lcd_picture[8][128];

void LcdKs0108::waitForLcdReady(uint8_t state) {
    //Линия RW = 1 - мы читаем с индикатора
//    digitalWrite(RW, 1);
    RW->switchOn();

    //Читаем команду
//    digitalWrite(CD, 0);
    CD->switchOff();

    //Включаем необходимый чип
    (state && CHIP1) ? E1->switchOn() : E2->switchOn();

    //Необходимая задержка
    Application::delayMks(1);

    //Выставляем строб
//    digitalWrite(E, 1);
    E->switchOn();
    //Необходимая задержка
    Application::delayMks(1);

    //Седьмой пин данных делаем входом
//    pinMode(dataPins[7], INPUT);
    dataPins[7]->oe(PORT_OE_IN);

    //Необходимая задержка
    Application::delayMks(10);

    //Дожидаемся сброса сигнала занятости
    while (dataPins[7]->read());

    //Сбрасываем строб
//    digitalWrite(E, 0);
    E->switchOff();

    //Вновь пин - на выход
//    pinMode(dataPins[7], OUTPUT);
    dataPins[7]->oe(PORT_OE_OUT);
    //Необходимая задержка
    Application::delayMks(1);

    //Сбрасываем сигналы управления чипами
//    digitalWrite(E1, 0);
//    digitalWrite(E2, 0);
    E1->switchOff();
    E2->switchOff();
}

uint8_t LcdKs0108::readByte(uint8_t state) {
    //Все пины - вход
    for (int i = 0; i < 8; ++i) {
//        pinMode(dataPins[i], INPUT);
        dataPins[i]->oe(PORT_OE_IN);
    }

    //Ждем готовности
    waitForLcdReady(state);

    //Будем считывать
//    digitalWrite(RW, 1);
//    digitalWrite(CD, 1);
    RW->switchOn();
    CD->switchOn();

    //Выбираем чип
    (state & CHIP1) ? E1->switchOn() : E2->switchOn();  //digitalWrite(E1, 1) : digitalWrite(E2, 1);

    //Необходимая задержка
    Application::delayMks(1);

    //Устанавливаем строб
//    digitalWrite(E, 1);
    E->switchOn();
    //Ждем
    Application::delayMks(1);

    uint8_t readedByte = 0;

    //Считываем данные с шины
    for (int i = 0; i < 8; ++i) {
        readedByte |= (dataPins[i]->read() << i);
    }

    //Убираем строб
//    digitalWrite(E, 0);
    E->switchOff();
    //Ждем
    Application::delayMks(1);

    //Восстанавливаем направление шины
    for (int i = 0; i < 8; ++i) {
//        pinMode(dataPins[i], OUTPUT);
        dataPins[i]->oe(PORT_OE_OUT);
    }
    return readedByte;
}

void LcdKs0108::writeByte(uint8_t data, uint8_t state) {

    //Ждем готовности
    waitForLcdReady(state);

    //На RW=0 - мы будем писАть в индикатор
//    digitalWrite(RW, 0);
    RW->switchOff();
    //Команду или данный
//    digitalWrite(CD, state & DATA);
    (state & DATA) ? CD->switchOn() : CD->switchOff();

    //Выбираем необходимый чип
    (state & CHIP1) ? E1->switchOn() : E1->switchOff(); //digitalWrite(E1, 1) : digitalWrite(E2, 1);
    (state & CHIP2) ? E2->switchOn() : E2->switchOff(); //digitalWrite(E1, 1) : digitalWrite(E2, 1);

    //Выставляем данные на шину
    for (int i = 0; i < 8; ++i) {
//        digitalWrite(dataPins[i], (data & (1 << i)));
        (data & (1 << i)) ? dataPins[i]->switchOn() : dataPins[i]->switchOff();
    }

    //Необходимая задержка
//    Application::delayMks(1);

    //Устанавливаем строб
//    digitalWrite(E, 1);
    E->switchOn();

    //Ждем
    Application::delayMks(1);

    //Сбрасываем строб
//    digitalWrite(E, 0);
    E->switchOff();

    //Снова ждем
//    Application::delayMks(1);

    //Сбрасываем сигналы управления чипами
//    digitalWrite(E1, 0);
//    digitalWrite(E2, 0);
    E1->switchOff();
    E2->switchOff();
}

void LcdKs0108::init(Pin *_DB0, Pin *_DB1, Pin *_DB2, Pin *_DB3,
                     Pin *_DB4, Pin *_DB5, Pin *_DB6, Pin *_DB7,
                     Pin *_E1, Pin *_E2, Pin *_RES, Pin *_RW,
                     Pin *_CD, Pin *_E) {

//    Serial.begin(115200);
    //Serial.println("init");

    //Заполняем массив пинами с линиями данных
    dataPins[0] = _DB0;
    dataPins[1] = _DB1;
    dataPins[2] = _DB2;
    dataPins[3] = _DB3;
    dataPins[4] = _DB4;
    dataPins[5] = _DB5;
    dataPins[6] = _DB6;
    dataPins[7] = _DB7;

    //Все пины - выход
    for (int i = 0; i < 8; ++i) {
        //Serial.println(dataPins[i], DEC);
        dataPins[i]->oe(PORT_OE_OUT);
    }
//    pinMode(_CD, OUTPUT);
    CD = _CD;
//    pinMode(_E, OUTPUT);
    E = _E;

//    pinMode(_E1, OUTPUT);
    E1 = _E1;
//    pinMode(_E2, OUTPUT);
    E2 = _E2;
//    pinMode(_RES, OUTPUT);
    RES = _RES;
//    pinMode(_RW, OUTPUT);
    RW = _RW;

    //Сбрасываем модуль
    E->switchOff();
    RES->switchOff();
//    digitalWrite(E, 0);
//    digitalWrite(RES, 0);
    Application::delayMs(2000);
//    digitalWrite(RES, 1);
    RES->switchOn();
    Application::delayMs(2000);


    writeByte(~0xC0, CHIP1);
    //Верхняя строчка отображения - 0
    writeByte(0xC0, CHIP1);
    writeByte(0xC0, CHIP2);

    //Включить контроллер!
    writeByte(0x3F, CHIP1);
    writeByte(0x3F, CHIP2);
}

void LcdKs0108::putPixel(uint8_t x, uint8_t y, bool bit) {
    if (bit) {
        lcd_picture[y / 8][x] |= 1 << (y % 8);
    } else {
        lcd_picture[y / 8][x] &= 0 << (y % 8);
    }
}

//void LcdKs0108::digitalWrite(Pin *pin, bool bit) {
//    if (bit) pin->switchOn();
//    else pin->switchOff();
//}

