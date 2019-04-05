/* i2c - Example

   For other examples please check:
   https://github.com/espressif/esp-idf/tree/master/examples

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include "driver/i2c.h"
#include "Draw_Functions.h"
#include "SSD1306.h"

/**
 * TEST CODE BRIEF
 *
 * This example will show you how to use MOD-OLED with ESP32-EVB.
 * https://github.com/OLIMEX/ESP32-EVB
 *
 * Pin assignment:
 *
 * - master:
 *    GPIO16 is assigned as the data signal of i2c master port
 *    GPIO13 is assigned as the clock signal of i2c master port
 *
 * Connection:
 * - connect MOD-OLED to the UEXT of the ESP32-EVB
 *
 *
 * - on the screen you will see a welcome message
 * - when you press the button you will see the button counter
 * - it will inverse the colors on every click
 */

uint8_t oledBuffer[OLED_SIZE];

SemaphoreHandle_t print_mux = NULL;

#define	PRESSED 	1
#define	RELEASED	0

// button scan function with simple debounce check
int Button_State (void)
{
	int Debounce_Timeout = 100;	// threshold value
	if (!gpio_get_level(GPIO_NUM_34))
	{
		while ((!gpio_get_level(GPIO_NUM_34)) && --Debounce_Timeout);
		if (!Debounce_Timeout)
			return PRESSED;
	}
	return RELEASED;
}

void i2c_test_task(void* arg)
{
	int ret, Flag=1, Counter=0;
	char Text [32];

	// button initialized as input
	gpio_set_direction (GPIO_NUM_34, GPIO_MODE_INPUT);
	
	oledInitialize ();
	print_text(1, " OLIMEX");
	print_smtext(3, (unsigned char *)"         Hello World!", 1);
	print_smtext(5, (unsigned char *)"         Press BUT1  ", 1);
	print_smtext(6, (unsigned char *)"         to see the  ", 1);
	print_smtext(7, (unsigned char *)"        click counter", 1);
	 

	drawLine(1, 25, 128, 25);

	drawEllipse(20, 70, 5, 20);
	drawLine(23, 52, 37, 60);
	drawLine(17, 52, 10, 64);

	drawCircle(20, 40, 10);
	drawLine(15, 38, 18, 38);
	drawLine(22, 38, 25, 38);
	drawLine(20, 40, 21, 42);
	drawLine(15, 43, 16, 45);
	drawLine(17, 46, 20, 47);
	drawTriangle(5, 35, 20, 28, 35, 35);

	drawRectangle(37, 55, 40, 65);
	drawSolidRectangle(37, 54, 40, 30);

	oledUpdate ();

	while (1)
	{
		ret = ESP_OK;
		xSemaphoreTake(print_mux, portMAX_DELAY);
		if(ret == ESP_OK)
		{
			if ((Button_State() == PRESSED) && Flag)
			{
				Counter++;
				Flag = 0;
				sprintf (Text, "BUT1 click count: %d", Counter);
				oledClear ();
				_set_inverse_display (Counter&1);
				print_text(1," OLIMEX");
				print_smtext(4,(unsigned char *)Text,0);
				oledUpdate ();
			}
			if (Button_State() == RELEASED)
				Flag = 1;
		}
		else
			if(ret == ESP_ERR_TIMEOUT)
			{
				printf("I2C timeout\n");
			}
			else
			{
				printf("No ack, sensor not connected...skip...\n");
			}
		xSemaphoreGive(print_mux);
	}
}

void app_main()
{
	print_mux = xSemaphoreCreateMutex();
	i2c_example_master_init();
	xTaskCreate(i2c_test_task, "i2c_test_task_1", 1024 * 2, (void* ) 1, 10, NULL);
}
