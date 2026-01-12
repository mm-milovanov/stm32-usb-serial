#include "bsp/bsp.h"
#include <system/system.h>

int main() {
    system_hdl_t hdl;

    Bsp_init();
    System_init(&hdl);

    while (1)
        System_step(&hdl);

    Bsp_deinit();

    return 0;
}