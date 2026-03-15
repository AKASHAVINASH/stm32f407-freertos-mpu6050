# STM32F407 FreeRTOS MPU6050 I2C Sensor Interface

## Overview

This project demonstrates how to interface the MPU6050 accelerometer and gyroscope sensor with the STM32F407 microcontroller using the I2C protocol. The application runs on FreeRTOS and periodically reads accelerometer data while sending debug messages over UART.

The system verifies the presence of the MPU6050 device using the WHO_AM_I register and then initializes the sensor before reading sensor values.

## Hardware Used

* STM32F407 microcontroller
* MPU6050 accelerometer and gyroscope module
* UART interface for debugging
* I2C interface for sensor communication

## Software Components

* STM32CubeIDE
* FreeRTOS
* STM32 HAL drivers
* I2C communication
* UART debug output

## Working Principle

1. The FreeRTOS Default Task starts after system initialization.
2. The program checks if the MPU6050 device is available on the I2C bus.
3. If detected, the WHO_AM_I register is read to confirm the sensor identity.
4. Once verified, the sensor is initialized:

   * The device is woken from sleep mode.
   * Sample rate divider is configured.
   * Accelerometer full-scale range is set to ±2g.
   * Gyroscope full-scale range is set to ±250°/s.
5. A status flag is set indicating that the sensor is ready.
6. The system enters a continuous loop where:

   * An LED toggles periodically.
   * Debug messages are transmitted over UART.
   * Accelerometer data is read from the MPU6050.

## Key Registers Used

WHO_AM_I Register (0x75)
Used to verify the identity of the MPU6050 device.

Power Management Register (0x6B)
Used to wake the sensor from sleep mode.

Sample Rate Divider (0x19)
Configures the internal sampling rate of the sensor.

Accelerometer Configuration (0x1C)
Sets accelerometer full scale range.

Gyroscope Configuration (0x1B)
Sets gyroscope full scale range.

## Features

* FreeRTOS based task scheduling
* I2C communication with MPU6050
* Accelerometer data acquisition
* UART debug output
* LED status indicator
* Device detection and initialization

## Example Output

ready to work
Rtos Running like baremetal

## Project Structure

Core/Src
Application source files

Core/Inc
Header files

Drivers
STM32 HAL drivers

Middlewares/FreeRTOS
RTOS kernel and configuration

## Future Improvements

* Add gyroscope data processing
* Implement sensor fusion algorithms
* Add UART data streaming
* Integrate Kalman or complementary filter
* Add interrupt driven sensor reading

## Author

Akash B

