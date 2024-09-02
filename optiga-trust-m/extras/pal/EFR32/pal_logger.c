/**
 * SPDX-FileCopyrightText: 2019-2024 Infineon Technologies AG
 * SPDX-License-Identifier: MIT
 *
 * \author Infineon Technologies AG
 *
 * \file    pal_logger.c
 *
 * \brief   This file provides the prototypes declarations for pal logger.
 *
 * \ingroup grPAL
 *
 * @{
 */

#include "pal_logger.h"
#include "em_i2c.h"

pal_logger_t logger_console =
{
    NULL,
    false,
    false,
};

pal_status_t pal_logger_init(void *p_logger_context) {
    /*
    if (p_logger == NULL) {
        return PAL_STATUS_FAILURE;
    }

    // I2C initialization struct
    I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;
    i2cInit.enable = true; // Enable I2C peripheral after initialization
    i2cInit.master = true; // Set to Controller mode
    i2cInit.refFreq = 0; // Use currently configured reference clock
    i2cInit.freq = I2C_FREQ_STANDARD_MAX; // Set to standard frequency
    i2cInit.clhr = i2cClockHLRStandard; // Use standard clock low/high ratio

    // Initialize I2C
    I2C_Init(I2C0, &i2cInit);

    // Store I2C peripheral information in logger context
    p_logger->logger_config_ptr = (void *)I2C0;

    // Initialize flags
    p_logger->logger_rx_flag = 0;
    p_logger->logger_tx_flag = 0;*/

    return PAL_STATUS_SUCCESS;
}

pal_status_t pal_logger_deinit(void *p_logger_context) {
    /*
    if (p_logger_context == NULL) {
        return PAL_STATUS_FAILURE;
    }

    pal_logger_t *p_logger = (pal_logger_t *)p_logger_context;
.
    I2C_Enable(I2C0, false);

    // Reset the logger context flags
    p_logger->logger_rx_flag = 0;
    p_logger->logger_tx_flag = 0;

    // Nullify the logger configuration pointer
    p_logger->logger_config_ptr = NULL;*/

    return PAL_STATUS_SUCCESS;
}

pal_status_t pal_logger_write(void *p_logger_context, const uint8_t *p_log_data, uint32_t log_data_length) {
    /* int32_t return_status = PAL_STATUS_FAILURE;
    pal_logger_t *p_log_context = p_logger_context;

    do {
        // !!!OPTIGA_LIB_PORTING_REQUIRED
    } while (0);
    return ((pal_status_t)return_status); */

    // Arduino: 

   /*  String str = (char*)p_log_data;

    for(int i=0; i< log_data_length; i++) {
        Serial.print(str.charAt(i));
    } */

    for(uint32_t i = 0; i < log_data_length; i++)
        printf("%c", p_log_data[i]); 

    printf("\n");

    return PAL_STATUS_SUCCESS;
}
/*
pal_status_t pal_logger_write(pal_logger_t *logger, const uint8_t *data, uint16_t length) {
    // Step 1: Initialize I2C
    I2C_Init_TypeDef i2cInit = {
        .enable = true,
        .master = true, // Assuming the logger acts as a master
        .refFreq = 0, // Use currently configured reference clock
        .freq = I2C_FREQ_STANDARD_MAX, // Use standard I2C frequency
        .clhr = i2cClockHLRStandard
    };
    I2C_Init(logger->i2c, &i2cInit);

    // Step 2: Start I2C transfer and address the logger device
    I2C_TransferSeq_TypeDef seq;
    seq.addr = logger->device_address << 1; // Left shift to make space for the read/write bit
    seq.flags = I2C_FLAG_WRITE;

    // Step 3: Write data
    seq.buf[0].data = (uint8_t *)data;
    seq.buf[0].len = length;

    I2C_TransferReturn_TypeDef result = I2C_TransferInit(logger->i2c, &seq);

    // Wait for the transfer to complete
    while (result == I2C_TransferInProgress) {
        result = I2C_Transfer(logger->i2c);
    }

    if (result == I2C_TransferDone) {
        // Transfer completed successfully
        return PAL_STATUS_SUCCESS; 
    } else {
        // Transfer failed
        return PAL_STATUS_FAILURE;
    }
}
*/

pal_status_t pal_logger_read(void *p_logger_context, uint8_t *p_log_data, uint32_t log_data_length) {

  /*
  int32_t return_status = PAL_STATUS_FAILURE;
    pal_logger_t *p_log_context = p_logger_context;

    do {
        // !!!OPTIGA_LIB_PORTING_REQUIRED
    } while (0);
    return ((pal_status_t)return_status);*/
    return PAL_STATUS_SUCCESS;
}
