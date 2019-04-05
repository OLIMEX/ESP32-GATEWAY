# ESP32-GATEWAY software development

This repo contains ESP-IDF examples for Olimex ESP32-GATEWAY.

Since there are major hardware changes between revision C and D, there
are two folders with examples! The clock source for the Ethernet is different!

If you own newer board, revision D or newer then get the IDF exmaples from
folder "Newest-hardware-revisions"

If you have board revision B or C then get the demos from folder
"Hardware-revisions-B-or-C"


If you are looking for Arduino demos just install the official ESP32 json,
our board is supported in the official branches.

## Install ESP-IDF
To run the examples in this directory or your own you'll need to get
SDK from Espressif called [ESP-IDF](https://github.com/espressif/esp-idf).
We recommend reading their
[setup guide](https://github.com/espressif/esp-idf#setting-up-esp-idf).

When IDF_PATH is in your PATH you're ready to continue with example running.

## Run examples
To run our example all you need to do is:

* Run menuconfig
```sh
    make menuconfig
```

    * Configure serial flasher

    ESP32-GATEWAY has built-in serial flasher to help users upload their projects.
    Connect the board to PC via USB-micro cable. New serial device should
    apear, for example **/dev/ttyUSB0**. Then in the menuconfig configure
    flasher as follows:

    ```
    Serial flasher config  --->
            (/dev/ttyUSB0) Default serial port
            Default baud rate (115200 baud) --->
            [*] Use compressed upload
            Flash SPI mode (DIO)  --->
            Flash SPI speed (40 MHz) --->
            Flash size (4 MB) --->
            [*] Detect flash size when flashing bootloader
            Before flashing (Reset to bootloader)  --->
            After flashing (Reset after flashing)  --->
            'make monitor' baud rate (115200 bps)  --->
    ```

    * Configure example

    Some examples have their own configuration menu. If it's present
    you can modify the options:

    ```
    Example Configuration  --->
            Select board relay to use in the example (Relay 1)  --->
    ```

Exit and save configuration.

* Compile example
To compile example run:
```sh
make
```

* Uploading example
As we previously mention uploading application is done automatically.
If serial flasher is configured do:
```sh
make flash
```

You'll see something like this:
```
esptool.py v2.0-beta3
Flashing binaries to serial port /dev/ttyUSB0 (app at offset 0x10000)...
esptool.py v2.0-beta3
Connecting........__
Uploading stub...
Running stub...
Stub running...
Configuring flash size...
Auto-detected Flash size: 4MB
Flash params set to 0x0220
Compressed 15808 bytes to 9219...
Wrote 15808 bytes (9219 compressed) at 0x00001000 in 0.8 seconds (effective 154.5 kbit/s)...
Hash of data verified.
Compressed 356080 bytes to 168292...
Wrote 356080 bytes (168292 compressed) at 0x00010000 in 14.9 seconds (effective 191.3 kbit/s)...
Hash of data verified.
Compressed 3072 bytes to 82...
Wrote 3072 bytes (82 compressed) at 0x00008000 in 0.0 seconds (effective 1905.4 kbit/s)...
Hash of data verified.

Leaving...
Hard resetting...
```

Optionally you can open serial monitor by typing:
```sh
make monitor
```
