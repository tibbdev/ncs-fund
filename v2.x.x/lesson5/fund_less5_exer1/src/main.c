/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Controlling LEDs through UART. Press 1-3 on your keyboard to toggle LEDS 1-3 on your development kit */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
/* STEP 3 - Include the header file of the UART driver in main.c */
#include <zephyr/drivers/uart.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   	(1000)

/* STEP 10.1.1 - Define the size of the receive buffer */
#define UART_RX_BUFF_SIZE	(10)

/* STEP 10.2 - Define the receiving timeout period */
#define UART_RX_TIMEOUT		(100)

/* STEP 5.1 - Get the device pointers of the LEDs through gpio_dt_spec */
#if defined (CONFIG_BOARD_NRF7002DK_NRF5340_CPUAPP)|| defined (CONFIG_BOARD_NRF7002DK_NRF5340_CPUAPP_NS)
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
#else
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);
static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(DT_ALIAS(led3), gpios);
#endif


/* STEP 4.1 - Get the device pointer of the UART hardware */
const struct device * dbg_uart = DEVICE_DT_GET(DT_NODELABEL(uart0));

/* STEP 9.1 - Define the transmission buffer, which is a buffer to hold the data to be sent over UART */
uint8_t tx_buff[] = {"nRF Connect SDK Fundamentals - Lesson 5 - Exercise 1\r\n"
"Press 1-3 on keyboard to toggle LEDS 1-3 on the board\r\n"};

/* STEP 10.1.2 - Define the receive buffer */
uint8_t rx_buf[UART_RX_BUFF_SIZE];

/* STEP 7 - Define the callback function for UART */
void uart_cb(const struct device * dev, struct uart_event * evt, void * user_data)
{
	switch (evt->type)
	{
		case UART_RX_RDY:
			if(1 == evt->data.rx.len)
			{
				uint8_t byte = evt->data.rx.buf[evt->data.rx.offset];

				if('1' == byte)
				{
					gpio_pin_toggle_dt(&led0);
				}
				else if('2' == byte)
				{
					gpio_pin_toggle_dt(&led1);
				}
				else if('3' == byte)
				{
					gpio_pin_toggle_dt(&led2);
				}
			}
			break;
		case UART_RX_DISABLED:
			// Enable continuous UART RX
			uart_rx_enable(dev, rx_buf, sizeof(rx_buf), UART_RX_TIMEOUT);
			break;
		
		default:
			break;
	}
	if(1 == evt->data.rx.len)
	{
		
	}
}

int main(void)
{
	int ret;

/* STEP 4.2 - Verify that the UART device is ready */ 
	if(!device_is_ready(dbg_uart))
	{
		printk("UART device not ready!!!\r\n");
		return 1;
	}

/* STEP 5.2 - Verify that the LED devices are ready */
	if (!device_is_ready(led0.port))
	{
		printk("GPIO device is not ready\r\n");
		return 1;
	}

/* STEP 6 - Configure the GPIOs of the LEDs */
	ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
	if(0 > ret)
	{
		return 1;
	}
	ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
	if(0 > ret)
	{
		return 1;
	}
	ret = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE);
	if(0 > ret)
	{
		return 1;
	}
	ret = gpio_pin_configure_dt(&led3, GPIO_OUTPUT_ACTIVE);
	if(0 > ret)
	{
		return 1;
	}

/* STEP 8 - Register the UART callback function */
	ret = uart_callback_set(dbg_uart, uart_cb, NULL);
	if (ret)
	{
		return 1;
	}
	

/* STEP 9.2 - Send the data over UART by calling uart_tx() */
	ret = uart_tx(dbg_uart, tx_buff, sizeof(tx_buff), SYS_FOREVER_US);
	if(ret)
	{
		return 1;
	}

/* STEP 10.3  - Start receiving by calling uart_rx_enable() and pass it the address of the receive  buffer */
	ret = uart_rx_enable(dbg_uart, rx_buf, sizeof(rx_buf), UART_RX_TIMEOUT);
	if(ret)
	{
		return 1;
	}

	while (1) {
		k_msleep(SLEEP_TIME_MS);
		gpio_pin_toggle_dt(&led3);
	}

}