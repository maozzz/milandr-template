#include "Application.h"
#include "common.h"
#include "core_cm3.h"


Application::Application() {
}

void Application::init() {
    initClk();
    initSystick();
    initIrq();
    ledInit();
//    initGLcd();
}

void Application::iterate() {
}

void Application::initSystick() {
    SysTick_Config(SysTick_MAX);
}

void Application::initGLcd() {
    glcd = new LcdKs0108();
    glcd->init(
            new Pin(MDR_PORTA, PORT_Pin_0, PORT_OE_OUT, PORT_SPEED_MAXFAST, PORT_MODE_DIGITAL, PORT_FUNC_PORT, 0, 0),
            new Pin(MDR_PORTA, PORT_Pin_1, PORT_OE_OUT, PORT_SPEED_MAXFAST, PORT_MODE_DIGITAL, PORT_FUNC_PORT, 0, 0),
            new Pin(MDR_PORTA, PORT_Pin_2, PORT_OE_OUT, PORT_SPEED_MAXFAST, PORT_MODE_DIGITAL, PORT_FUNC_PORT, 0, 0),
            new Pin(MDR_PORTA, PORT_Pin_3, PORT_OE_OUT, PORT_SPEED_MAXFAST, PORT_MODE_DIGITAL, PORT_FUNC_PORT, 0, 0),
            new Pin(MDR_PORTA, PORT_Pin_4, PORT_OE_OUT, PORT_SPEED_MAXFAST, PORT_MODE_DIGITAL, PORT_FUNC_PORT, 0, 0),
            new Pin(MDR_PORTA, PORT_Pin_5, PORT_OE_OUT, PORT_SPEED_MAXFAST, PORT_MODE_DIGITAL, PORT_FUNC_PORT, 0, 0),
            new Pin(MDR_PORTF, PORT_Pin_2, PORT_OE_OUT, PORT_SPEED_MAXFAST, PORT_MODE_DIGITAL, PORT_FUNC_PORT, 0, 0),
            new Pin(MDR_PORTF, PORT_Pin_3, PORT_OE_OUT, PORT_SPEED_MAXFAST, PORT_MODE_DIGITAL, PORT_FUNC_PORT, 0, 0),
            new Pin(MDR_PORTB, PORT_Pin_7, PORT_OE_OUT, PORT_SPEED_MAXFAST, PORT_MODE_DIGITAL, PORT_FUNC_PORT, 0, 0),
            new Pin(MDR_PORTB, PORT_Pin_8, PORT_OE_OUT, PORT_SPEED_MAXFAST, PORT_MODE_DIGITAL, PORT_FUNC_PORT, 0, 0),
            new Pin(MDR_PORTB, PORT_Pin_9, PORT_OE_OUT, PORT_SPEED_MAXFAST, PORT_MODE_DIGITAL, PORT_FUNC_PORT, 0, 0),
            new Pin(MDR_PORTB, PORT_Pin_10, PORT_OE_OUT, PORT_SPEED_MAXFAST, PORT_MODE_DIGITAL, PORT_FUNC_PORT, 0, 0),
            new Pin(MDR_PORTC, PORT_Pin_0, PORT_OE_OUT, PORT_SPEED_MAXFAST, PORT_MODE_DIGITAL, PORT_FUNC_PORT, 0, 0),
            new Pin(MDR_PORTC, PORT_Pin_1, PORT_OE_OUT, PORT_SPEED_MAXFAST, PORT_MODE_DIGITAL, PORT_FUNC_PORT, 0, 0)
    );
}

void Application::systick() {
    systickRounds++;
    led->toggle();
}

void Application::ledInit() {
    led = new Pin(MDR_PORTC, PORT_Pin_0);
    led->init()->oe(PORT_OE_OUT)->speed(PORT_SPEED_MAXFAST)->mode(PORT_MODE_DIGITAL);
}

void Application::initIrq() {
    __enable_irq();
    NVIC_EnableIRQ(SysTick_IRQn);
    NVIC_EnableIRQ(WWDG_IRQn);
}

void Application::initClk() {
    // HSE ON
    RST_CLK_DeInit();
    SystemInit();

    RST_CLK_HSEconfig(RST_CLK_HSE_ON);
    while (RST_CLK_HSEstatus() != SUCCESS);

    // FLASH MEMORY
    MDR_EEPROM->CMD |= 0b111000;

    // PLL ON
    RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul10);
    RST_CLK_CPU_PLLcmd(ENABLE);
    while (RST_CLK_CPU_PLLstatus() != SUCCESS);

    RST_CLK_CPU_PLLuse(ENABLE);
    RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);
    RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);

    SystemCoreClockUpdate();
}

uint16_t Application::systickRounds = 0;

void Application::delayMks(uint32_t mks) {
    systickRounds = 0;
    uint32_t start = SysTick_MAX - SysTick->VAL;
    uint64_t w8Ticks = SystemCoreClock / 1000000 * mks;
    uint64_t i;
    do {
        i = SysTick_MAX * systickRounds + SysTick_MAX - SysTick->VAL - start;
    } while (i < w8Ticks);
}

void Application::delayMs(uint32_t ms) {
    Application::delayMks(ms * 1000);
}

