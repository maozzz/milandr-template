#include "Application.h"

Application *app;

int main() {
    app = new Application();
    app->init();

    while (1) {
        app->iterate();
    }
}

#ifdef __cplusplus
extern "C" {
#endif

void SysTick_Handler(void) {
    app->systick();
}

void Default_Handler(void) {
    asm("nop");
}

void WWDG_IRQHandler(void) {
    asm("nop");
}

#ifdef __cplusplus
} // extern "C" block end
#endif