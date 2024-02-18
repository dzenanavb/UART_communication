# UART communication between ESP32 and FRDM-KL25z

This example shows how to use UART communicaton, and use MQTT to ensure a stable serial communication between two microcontrolers, when one of them does not offer WiFi module.

You can build this project with all supported [Mbed OS build tools](https://os.mbed.com/docs/mbed-os/latest/tools/index.html). However, this example project specifically refers to the command-line interface tool [Arm Mbed CLI](https://github.com/ARMmbed/mbed-cli#installing-mbed-cli).

1. Install Mbed CLI.
1. From the command-line, import the example: `mbed import mbed-os-example-blinky-baremetal`
1. Change the current directory to where the project was imported.

Alternatively, you can manually copy the binary to the target, which gets mounted on the host computer through USB.

Depending on the target, you can build the example project with the `GCC_ARM`, `ARM` or `IAR` toolchain. After installing Arm Mbed CLI, run the command below to determine which toolchain supports your target:
