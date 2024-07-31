/**
 * SPDX-FileCopyrightText: 2019-2024 Infineon Technologies AG
 * SPDX-License-Identifier: MIT
 *
 * \author Infineon Technologies AG
 *
 * \file pal_ifx_i2c_config.c
 *
 * \brief   This file implements platform abstraction layer configurations for ifx i2c protocol.
 *
 * \ingroup  grPAL
 *
 * @{
 */

#include "ifx_i2c_config.h"
#include "pal_gpio.h"
#include "pal_i2c.h"

#define RESET_PIN 3
#define SDA_PIN 12
#define SCL_PIN 13
#define VDD_PIN 18

// Define the structure for I2C master configuration
struct locl_i2c_struct_to_descroibe_master {
    uint32_t refFreq; // Reference frequency for I2C
    uint32_t freq; // Desired I2C bus frequency
    int master; // Set to true for master mode
    uint8_t sda_pin; // GPIO pin for SDA
    uint8_t scl_pin; // GPIO pin for SCL
    // Add other necessary parameters here
};

// Instantiate and initialize the I2C master configuration
struct locl_i2c_struct_to_descroibe_master i2c_master_0 = {
    .refFreq = 0, // Use currently configured reference clock
    .freq = 400000, // Set I2C speed to 400 kHz
    .master = 1, // Configure as master
    .sda_pin = SDA_PIN, // SDA GPIO pin
    .scl_pin = SCL_PIN, // SCL GPIO pin
    // Initialize other parameters as needed
};

// Complete the PAL I2C context configuration for OPTIGA
pal_i2c_t optiga_pal_i2c_context_0 = {
    .p_i2c_hw_config = (void *)&i2c_master_0, // Point to the I2C master config
    .p_upper_layer_ctx = NULL, // To be set by the application
    .upper_layer_event_handler = NULL, // To be set by the application
    .slave_address = 0x30, // Set the slave address (example)
};

/**
 * \brief PAL vdd pin configuration for OPTIGA.
 */
pal_gpio_t optiga_vdd_0 = {
    // Platform specific GPIO context for the pin used to toggle Vdd.
    (void *)VDD_PIN};

/**
 * \brief PAL reset pin configuration for OPTIGA.
 */
pal_gpio_t optiga_reset_0 = {
    // Platform specific GPIO context for the pin used to toggle Reset.
    (void *)RESET_PIN};
