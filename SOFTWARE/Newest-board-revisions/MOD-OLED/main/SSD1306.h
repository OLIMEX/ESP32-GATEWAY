#ifndef	_SSD1306_H
#define	_SSD1306_H

#include <string.h>
#include "driver/i2c.h"


#define I2C_EXAMPLE_MASTER_SCL_IO          16               /*!< gpio number for I2C master clock */
#define I2C_EXAMPLE_MASTER_SDA_IO          32               /*!< gpio number for I2C master data  */
#define I2C_EXAMPLE_MASTER_NUM             I2C_NUM_1        /*!< I2C port number for master dev */
#define I2C_EXAMPLE_MASTER_TX_BUF_DISABLE  0                /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_RX_BUF_DISABLE  0                /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_FREQ_HZ         1000000          /*!< I2C master clock frequency */

#define OLED_ADDRESS    0x3C

#define WRITE_BIT                          I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT                           I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN                       0x1              /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS                      0x0              /*!< I2C master will not check ack from slave */
#define ACK_VAL                            0x0              /*!< I2C ack value */
#define NACK_VAL                           0x1              /*!< I2C nack value */

#define WIDTH           128
#define HEIGHT          64
#define LCD_X_RES WIDTH
#define LCD_Y_RES HEIGHT
#define OLED_SIZE       (WIDTH * HEIGHT / 8)

extern uint8_t oledBuffer[OLED_SIZE];

void i2c_example_master_init();

void _i2cWrite(uint8_t reg, uint8_t *data, uint32_t size);

void _sendCommand(uint8_t *data, uint32_t size);

void _sendData(uint8_t *data, uint32_t size);

void _set_display_on_off(bool state);

void _set_multiplex_ratio(uint8_t ratio);

void _set_display_offset(uint8_t offset);

void _set_display_start_line(uint8_t line);

void _set_segment_remap(uint8_t remap);

void _set_com_scan_direction(uint8_t dir);

void _set_com_pins_configuration(uint8_t conf, uint8_t remap);

void _set_contrast_control(uint8_t contrast);

void _entire_display_on(uint8_t status);

void _set_inverse_display(uint8_t inverse);

void _set_display_clock(uint8_t divider, uint8_t freq);

void _charge_pump_settings(uint8_t on);

void _set_memory_addressing_mode(uint8_t mode);

void _set_column_address(uint8_t start, uint8_t end);

void _set_page_address(uint8_t start, uint8_t end);

void oledInitialize(void);

void oledUpdate(void);

void oledClear(void);

#endif
