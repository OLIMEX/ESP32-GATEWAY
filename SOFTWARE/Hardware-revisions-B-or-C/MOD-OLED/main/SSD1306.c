#include "SSD1306.h"


/**
 * @brief i2c master initialization
 */
void i2c_example_master_init()
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

void _i2cWrite(uint8_t reg, uint8_t *data, uint32_t size)
{
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, OLED_ADDRESS << 1 | WRITE_BIT, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);
	i2c_master_write(cmd, data, size, ACK_CHECK_EN);
	i2c_master_stop(cmd);
	//i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
	i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 0);
	i2c_cmd_link_delete(cmd);
}

void _sendCommand(uint8_t *data, uint32_t size)
{
	_i2cWrite(0x00, data, size);
}

void _sendData(uint8_t *data, uint32_t size)
{
	_i2cWrite(0x40, data, size);
}

void _set_display_on_off(bool state)
{
	uint8_t command[1] = { 0xae | (uint8_t)state };
	_sendCommand(command, sizeof(command));
}

void _set_multiplex_ratio(uint8_t ratio)
{
	uint8_t command[2] = { 0xa8, ratio };
	_sendCommand(command, sizeof(command));
}

void _set_display_offset(uint8_t offset)
{
	uint8_t command[2] = { 0xd3, offset };
	_sendCommand(command, sizeof(command));
}

void _set_display_start_line(uint8_t line)
{
	uint8_t command[1] = { 0x40 | line };
	_sendCommand(command, sizeof(command));
}

void _set_segment_remap(uint8_t remap)
{
	uint8_t command[1] = { 0xa0 | remap };
	_sendCommand(command, sizeof(command));
}

void _set_com_scan_direction(uint8_t dir)
{
	uint8_t command[1] = { 0xc0 | dir << 3 };
	_sendCommand(command, sizeof(command));
}

void _set_com_pins_configuration(uint8_t conf, uint8_t remap)
{
	uint8_t command[2] = { 0xda, 0x02 | (conf << 4) | (remap << 5) };
	_sendCommand(command, sizeof(command));
}

void _set_contrast_control(uint8_t contrast)
{
	uint8_t command[2] = { 0x81, contrast };
	_sendCommand(command, sizeof(command));
}

void _entire_display_on(uint8_t status)
{
	uint8_t command[1] = { 0xa4 | status };
	_sendCommand(command, sizeof(command));
}

void _set_inverse_display(uint8_t inverse)
{
	uint8_t command[1] = { 0xa6 | inverse };
	_sendCommand(command, sizeof(command));
}

void _set_display_clock(uint8_t divider, uint8_t freq)
{
	uint8_t command[2] = { 0xd5, (freq << 4 | divider) };
	_sendCommand(command, sizeof(command));
}

void _charge_pump_settings(uint8_t on)
{
	uint8_t command[2] = { 0x8d, 0x10 | (on << 2) };
	_sendCommand(command, sizeof(command));
}

void _set_memory_addressing_mode(uint8_t mode)
{
	uint8_t command[2] = { 0x20, mode };
	_sendCommand(command, sizeof(command));
}

void _set_column_address(uint8_t start, uint8_t end)
{
	uint8_t command[3] = { 0x21, start, end };
	_sendCommand(command, sizeof(command));
}

void _set_page_address(uint8_t start, uint8_t end)
{
	uint8_t command[3] = { 0x22, start, end };
	_sendCommand(command, sizeof(command));
}

void oledInitialize(void)
{
	/* Send initialization */
	_set_display_on_off(false);
	_set_multiplex_ratio(63);
	_set_display_offset(0);
	_set_display_start_line(0);

	_set_segment_remap(1);

	_set_com_pins_configuration(1, 0);
	_set_com_scan_direction(1);

	_set_contrast_control(0x7f);
	_entire_display_on(0);
	_set_inverse_display(0);
	_set_display_clock(0, 15);
	_charge_pump_settings(1);
	_set_display_on_off(true);

	_set_memory_addressing_mode(0);
	_set_column_address(0, 127);
	_set_page_address(0, 7);

	/* Clear display */
	oledClear();
	oledUpdate();
}

void oledUpdate(void)
{
	_sendData(oledBuffer, OLED_SIZE);
}

void oledClear(void)
{
	memset(oledBuffer, 0, OLED_SIZE);
}
