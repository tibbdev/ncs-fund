/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>

/* STEP 7 - Change the sleep time from 1000 ms to 100 ms */
#define SLEEP_TIME_MS   100

/* STEP 3.1 - Get the node identifier for button 1 through its alias sw0 */
#define SW0_NODE	DT_ALIAS(sw0)
#define SW1_NODE	DT_ALIAS(sw1)

/* STEP 3.2 - Get the device pointer. pin number, and pin's configuration flags through gpio_dt_spec */
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW0_NODE, gpios);
static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET(SW1_NODE, gpios);

/* LED0_NODE is the devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int main(void)
{
	int ret;

	if (!device_is_ready(led.port)) {
		return 0;
	}
	/* STEP 4 - Verify that the device is ready for use */
	if (!device_is_ready(button.port)) {
		return 0;
	}
	if (!device_is_ready(button1.port)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	/* STEP 5 - Configure the pin connected to the button to be an input pin and set its hardware specifications */
	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret < 0) {
		return 0;
	}
	ret = gpio_pin_configure_dt(&button1, GPIO_INPUT);
	if (ret < 0) {
		return 0;
	}

	bool b0, b1, out = false;

	while (1) {
		/* STEP 6.1 - Read the status of the button and store it */
		b0 = gpio_pin_get_dt(&button);
		b1 = gpio_pin_get_dt(&button1);

		if(b0)
		{
			out = true;
		}
		else if(b1)
		{
			out = false;
		}

		/* STEP 6.2 - Update the LED to the status of the button */
		gpio_pin_set_dt(&led, out);

		k_msleep(SLEEP_TIME_MS); // Put the main thread to sleep for 100ms for power optimization
	}
}