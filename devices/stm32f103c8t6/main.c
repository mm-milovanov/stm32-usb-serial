#include "bsp/bsp.h"
// #include <system/system.h>

int main() {
    // system_hdl_t hdl;

    Bsp_init();
    // System_init(&hdl);

    while(1) {
        // System_step(&hdl);
        SysTick_delay(1000);
        gpio_pin_hdl_write(&__LED__, GPIO_PIN_SET);
        SysTick_delay(1000);
        gpio_pin_hdl_write(&__LED__, GPIO_PIN_RESET);
    }

    Bsp_deinit();

    return 0;
}