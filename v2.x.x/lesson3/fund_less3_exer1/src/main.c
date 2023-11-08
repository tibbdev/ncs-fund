#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#ifdef CONFIG_MYFUNCTION
    #include "func.h"
#endif

void main(void)
{
    #ifdef CONFIG_MYFUNCTION
        int a = 1;
        int b = 2;
        int temp;
    #endif

    while(1)
    {
        #ifdef CONFIG_MYFUNCTION
            printk("\r\nThe sum of %d and %d is %d", a, b, sum(a, b));
            temp = a;
            a = sum(a, b) % 512;
            b++;
        #else
            printk("\r\nMYFUNCTION not enabled");
        #endif
        k_msleep(1000);
    }
}