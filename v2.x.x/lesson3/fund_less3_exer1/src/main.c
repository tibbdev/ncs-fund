#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

void main(void)
{
    while(1)
    {
        printk("Hello World!\r\n");
        k_msleep(1000);
    }
}