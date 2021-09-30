# Ethernet Example
(See the README.md file in the upper level 'examples' directory for more information about examples.)

## Overview

This example demonstrates basic usage of `Ethernet driver` together with `tcpip_adapter`. The work flow of the example could be as follows:

1. Install Ethernet driver
2. Send DHCP requests and wait for a DHCP lease
3. If get IP address successfully, then you will be able to ping the device

If you have a new Ethernet application to go (for example, connect to IoT cloud via Ethernet), try this as a basic template, then add your own code.

## How to use example

### Hardware Required

To run this example, it's recommended that you have the Olimex board ESP32-GATEWAY or one of its variants. This example should also work for 3rd party ESP32 board as long as it's integrated with a supported Ethernet PHY chip. Up until now, ESP-IDF supports up to four Ethernet PHY: LAN8720, IP101, DP83848 and RTL8201, additional PHY drivers should be implemented by users themselves.

Notice that Olimex board uses LAN8710 chip but the libraries for LAN8710 and LAN8720 are similar (the chips are very similar).

Besides that, esp_eth component can drive third-party Ethernet module which integrates MAC and PHY and provides common communication interface (e.g. SPI, USB, etc).This example will take the LAN8720 library for the an example, illustrating how to install the Ethernet driver in the same manner. In this project by default this feature is disabled. If you want to enable it:

```
idf.py menuconfig
```

then option and navigate to "Component config -> Ethernet" and check the "Support SPI to Ethernet Module" and then in that submenu check "Use LAN8720".

### Configure the project

```
idf.py menuconfig
```

See common configurations for Ethernet examples from [upper level](../../README.md#run-examples).

### Build, Flash, and Run

Build the project and flash it to the board, then run monitor tool to view serial output:

```
idf.py -p PORT build flash monitor
```

(Replace PORT with the name of the serial port to use.)

(To exit the serial monitor, type ``Ctrl-]``.)

See the [Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html) for full steps to configure and use ESP-IDF to build projects.

## Example Output

```bash
I (394) eth_example: Ethernet Started
I (3934) eth_example: Ethernet Link Up
I (3934) eth_example: Ethernet HW Addr 30:ae:a4:c6:87:5b
I (5864) tcpip_adapter: eth ip: 192.168.2.151, mask: 255.255.255.0, gw: 192.168.2.2
I (5864) eth_example: Ethernet Got IP Address
I (5864) eth_example: ~~~~~~~~~~~
I (5864) eth_example: ETHIP:192.168.2.151
I (5874) eth_example: ETHMASK:255.255.255.0
I (5874) eth_example: ETHGW:192.168.2.2
I (5884) eth_example: ~~~~~~~~~~~
```

Now you can ping your ESP32 in the terminal by entering `ping 192.168.2.151` (it depends on the actual IP address you get).

## Troubleshooting

For any technical queries, please open an [issue](https://github.com/OLIMEX/ESP32-GATEWAY/issues) on GitHub. We will get back to you as soon as possible.
