// Copyright (c) 2017 Olimex LTD.
//
// GNU GENERAL PUBLIC LICENSE
//    Version 3, 29 June 2007
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <stdio.h>
#include "driver/gpio.h"
#include "sdkconfig.h"

#define OLIMEX_LED_PIN      33
#define OLIMEX_BUT_PIN      34


/**
 * @brief gpio_isr_handler  Handle button press interrupt
 */
static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    gpio_set_level(OLIMEX_LED_PIN, !gpio_get_level(OLIMEX_BUT_PIN));
}

/**
 * @brief app_main          Application entry point
 */
void app_main()
{

    /* Make pads GPIO */
    gpio_pad_select_gpio(OLIMEX_LED_PIN);
    gpio_pad_select_gpio(OLIMEX_BUT_PIN);

    /* Set the Relay as a push/pull output */
    gpio_set_direction(OLIMEX_LED_PIN, GPIO_MODE_OUTPUT);

    /* Set Button as input */
    gpio_set_direction(OLIMEX_BUT_PIN, GPIO_MODE_INPUT);
    /* Enable interrupt on both edges */
    gpio_set_intr_type(OLIMEX_BUT_PIN, GPIO_INTR_ANYEDGE);

    /* Install ISR routine */
    gpio_install_isr_service(ESP_INTR_FLAG_LOWMED);
    gpio_isr_handler_add(OLIMEX_BUT_PIN, gpio_isr_handler, 0);



}
