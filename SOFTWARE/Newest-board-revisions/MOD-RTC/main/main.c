/* i2c - Example

   For other examples please check:
   https://github.com/espressif/esp-idf/tree/master/examples

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "driver/i2c.h"

/**
 * TEST CODE BRIEF
 *
 * This example will show you how to use I2C module.
 *
 * - read external i2c module, here we use a PCF8563
 *
 * Pin assignment:
 *
 * - master:
 *    GPIO32 is assigned as the data signal of i2c master port
 *    GPIO16 is assigned as the clock signal of i2c master port
 *
 * Connection:
 * - connect MOD-OLED to ESP32-GATEWAY using
 *   jumper wires (as shown on the picture)
 *   or any other kind of connections.
 * by default:
 *  MOD-RTC UEXT --> ESP32-GATEWAY extension
 *  pin 1 (3.3V) -->  pin 1 (3.3V)
 *  pin 2 (GND)  -->  pin 2  (GND)
 *  pin 5 (SCL)  -->  pin 11 (GPIO 16)
 *  pin 6 (SDA)  -->  pin 13 (GPIO 32)
 *
 *
 * - read the data from the module, if connected.
 * - on the terminal date and time will be printed
 * - by default the date and time is set to 28 February 2020; 23:59:50
 */

#define I2C_EXAMPLE_MASTER_SCL_IO          16               /*!< gpio number for I2C master clock */
#define I2C_EXAMPLE_MASTER_SDA_IO          32               /*!< gpio number for I2C master data  */
#define I2C_EXAMPLE_MASTER_NUM             I2C_NUM_1        /*!< I2C port number for master dev */
#define I2C_EXAMPLE_MASTER_TX_BUF_DISABLE  0                /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_RX_BUF_DISABLE  0                /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_FREQ_HZ         100000           /*!< I2C master clock frequency */

// I2C address of the module
#define  ADDRESS 0x51

// Control and status registers
#define  CTRL_STAT1  0x00
#define  CTRL_STAT2  0x01

// Time and date registers
#define  SECONDS     0x02
#define  MINUTES     0x03
#define  HOURS       0x04
#define  DAYS        0x05
#define  WEEKDAYS    0x06
#define  MONTHS      0x07
#define  YEARS       0x08

// Alarm registers
#define  AMINUTES    0x09
#define  AHOURS      0x0A
#define  ADAYS       0x0B
#define  AWEEKDAYS   0x0C

//CLKOUT control register
#define  CLKOUT_CTRL 0x0D

// Timer registers
#define  TIMER_CTRL  0x0E
#define  TIMER       0x0F

#define WRITE_BIT                          I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT                           I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN                       0x1              /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS                      0x0              /*!< I2C master will not check ack from slave */
#define ACK_VAL                            0x0              /*!< I2C ack value */
#define NACK_VAL                           0x1              /*!< I2C nack value */

// 28 february 2020; 23:59:50
#define	INITIAL_DATE_TIME	{20, 2, 28, 5, 23, 59, 50}

struct _Date_Time
{
	unsigned char Year, Month, Day, WeekDay, Hour, Minute, Second;
};
typedef struct _Date_Time Date_Time;

SemaphoreHandle_t print_mux = NULL;

const int Mask[16] =
{
	0xFF,  // 0
	0xFF,  // 1
	0x7F,  // 2
	0x7F,  // 3
	0x3F,  // 4
	0x3F,  // 5
	0x07,  // 6
	0x1F,  // 7
	0xFF,  // 8
	0x7F,  // 9
	0x3F,  // 10
	0x3F,  // 11
	0x07,  // 12
	0x03,  // 13
	0x03,  // 14
	0xFF,  // 15
};

const char WeekDays[7][10] =
{
	"Sunday",
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday"
};

int BCD_to_Int (int Input)
{
	return (Input>>4)*10+(Input&0x0F);
}

int Int_to_BCD (int Input)
{
	return ((Input/10)<<4) | (Input%10);
}

/**
 * @brief i2c master initialization
 */
static void i2c_example_master_init()
{
	int i2c_master_port = I2C_EXAMPLE_MASTER_NUM;
	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = I2C_EXAMPLE_MASTER_SDA_IO;
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_io_num = I2C_EXAMPLE_MASTER_SCL_IO;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	conf.master.clk_speed = I2C_EXAMPLE_MASTER_FREQ_HZ;
	i2c_param_config(i2c_master_port, &conf);
	i2c_driver_install(i2c_master_port, conf.mode, I2C_EXAMPLE_MASTER_RX_BUF_DISABLE, I2C_EXAMPLE_MASTER_TX_BUF_DISABLE, 0);
}

static esp_err_t i2c_example_master_sensor_test(i2c_port_t i2c_num, uint8_t* Data)
{
	int ret;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, ADDRESS << 1 | WRITE_BIT, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, CTRL_STAT1, ACK_CHECK_EN);
	i2c_master_stop(cmd);
	ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);
	if (ret != ESP_OK)
	{
		return ret;
	}
	vTaskDelay(30 / portTICK_RATE_MS);
	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, ADDRESS << 1 | READ_BIT, ACK_CHECK_EN);
	i2c_master_read(cmd, Data, 15, ACK_VAL);
	i2c_master_read_byte(cmd, Data+15, NACK_VAL);
	i2c_master_stop(cmd);
	ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);

	return ret;
}

void I2C_Send (unsigned char Register, unsigned char Data)
{
	unsigned char Temp[2] = {Register, Data};
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, ADDRESS << 1 | WRITE_BIT, ACK_CHECK_EN);
	i2c_master_write(cmd, Temp, 2, ACK_CHECK_EN);
	i2c_master_stop(cmd);
	i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);
}

void Set_Date_Time (Date_Time *Start_Date_Time)
{
	I2C_Send (YEARS, Int_to_BCD(Start_Date_Time->Year));
	I2C_Send (MONTHS, Int_to_BCD(Start_Date_Time->Month));
	I2C_Send (DAYS, Int_to_BCD(Start_Date_Time->Day));
	I2C_Send (WEEKDAYS, Int_to_BCD(Start_Date_Time->WeekDay));
	I2C_Send (HOURS, Int_to_BCD(Start_Date_Time->Hour));
	I2C_Send (MINUTES, Int_to_BCD(Start_Date_Time->Minute));
	I2C_Send (SECONDS, Int_to_BCD(Start_Date_Time->Second));

	return;
}

static void i2c_test_task(void* arg)
{
	unsigned char Data[16];
	int ret, i;
	Date_Time Start_Date_Time = INITIAL_DATE_TIME;
	Set_Date_Time (&Start_Date_Time);
	while (1)
	{
		ret = i2c_example_master_sensor_test( I2C_EXAMPLE_MASTER_NUM, Data);
		xSemaphoreTake(print_mux, portMAX_DELAY);
		if(ret == ESP_ERR_TIMEOUT)
		{
			printf("I2C timeout\n");
		}
		else if(ret == ESP_OK)
		{
			// raw data
			// for (i=0; i<16; i++)
				//printf ("Data[%d] = %x\n", i, Data[i]);
			for (i=0; i<16; i++)
				Data[i] = BCD_to_Int(Data[i] & Mask[i]);
			printf ("\f%.2d/%.2d/%.2d (%s) %.2d:%.2d:%.2d\n", Data[8], Data[7], Data[5], WeekDays[Data[6]], Data[4], Data[3], Data[2]);
		}
		else
		{
			printf("No ack, sensor not connected...skip...\n");
		}
		xSemaphoreGive(print_mux);
		vTaskDelay(50);	// equals ~half a seconds
	}
}

void app_main()
{
	print_mux = xSemaphoreCreateMutex();
	i2c_example_master_init();
	xTaskCreate(i2c_test_task, "i2c_test_task_1", 1024 * 2, (void* ) 1, 10, NULL);
}
