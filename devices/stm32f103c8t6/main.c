#include <bsp/bsp.h>
#include <system/system.h>

// int _write(int file, char *ptr, int len)
// {
//     for (int i=0; i<len; i++)
//         ITM_SendChar(*ptr++);
//     return len;
// }

int main(void){
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    /* set CP10 and CP11 Full Access */
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2)); 
#endif

    system_hdl_t hdl;

    __bsp_init__();
    System_init(&hdl);

    while(1)
        System_step(&hdl);

    return 0;
}