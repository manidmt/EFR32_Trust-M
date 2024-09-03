/**
 * SPDX-FileCopyrightText: 2019-2024 Infineon Technologies AG
 * SPDX-License-Identifier: MIT
 *
 * \author Infineon Technologies AG
 *
 * \file pal_i2c.c
 *
 * \brief   This file implements the platform abstraction layer(pal) APIs for I2C.
 *
 * \ingroup  grPAL
 *
 * @{
 */

#include "pal_i2c.h"
#include "em_i2c.h"
#include "optiga_lib_config.h"
#include "em_cmu.h" // For clock management
#include "sl_i2cspm.h"

#include "queue.h"
#include "semphr.h"

#define PAL_I2C_MASTER_MAX_BITRATE (400U)

static volatile uint32_t g_entry_count = 0;
static pal_i2c_t *gp_pal_i2c_current_ctx;

QueueHandle_t trustx_i2cresult_queue;

typedef struct i2c_result {
    /// Pointer to store upper layer callback context (For example: Ifx i2c context)
    pal_i2c_t *i2c_ctx;
    /// I2C Transmission result (e.g. PAL_I2C_EVENT_SUCCESS)
    uint16_t i2c_result;
} i2c_result_t;

TaskHandle_t xIicCallbackTaskHandle = NULL;
SemaphoreHandle_t xIicSemaphoreHandle;

// I2C acquire bus function
// lint --e{715} suppress the unused p_i2c_context variable lint error , since this is kept for future enhancements
static pal_status_t pal_i2c_acquire(const void *p_i2c_context) {

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (xSemaphoreTakeFromISR(xIicSemaphoreHandle, &xHigherPriorityTaskWoken) == pdTRUE)
        return PAL_STATUS_SUCCESS;
    else
        return PAL_STATUS_FAILURE;
//    (void)p_i2c_context;
//
//    if (0 == g_entry_count) {
//        g_entry_count++;
//        if (1 == g_entry_count) {
//            return PAL_STATUS_SUCCESS;
//        }
//    }
//    return PAL_STATUS_FAILURE;
}

// I2C release bus function
// lint --e{715} suppress the unused p_i2c_context variable lint, since this is kept for future enhancements
static void pal_i2c_release(const void *p_i2c_context) {

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xSemaphoreGiveFromISR(xIicSemaphoreHandle, &xHigherPriorityTaskWoken);

//    (void)p_i2c_context;
//
//    g_entry_count = 0;
}

void invoke_upper_layer_callback(const pal_i2c_t *p_pal_i2c_ctx, optiga_lib_status_t event) {
    upper_layer_callback_t upper_layer_handler;

    upper_layer_handler = (upper_layer_callback_t)p_pal_i2c_ctx->upper_layer_event_handler;

    upper_layer_handler(p_pal_i2c_ctx->p_upper_layer_ctx, event);

    // Release I2C Bus
    pal_i2c_release(p_pal_i2c_ctx->p_upper_layer_ctx);
}

// !!!OPTIGA_LIB_PORTING_REQUIRED
// The next 5 functions are required only in case you have interrupt based i2c implementation
void i2c_master_end_of_transmit_callback(void) {

    i2c_result_t i2_result;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    i2_result.i2c_ctx = gp_pal_i2c_current_ctx;
    i2_result.i2c_result = PAL_I2C_EVENT_SUCCESS;

    /*
     * You cann't call callback from the timer callback, this might lead to a corruption
     * Use queues instead to activate corresponding handler
     * */
    xQueueSendFromISR(trustx_i2cresult_queue, (void *)&i2_result, &xHigherPriorityTaskWoken);
  //invoke_upper_layer_callback(gp_pal_i2c_current_ctx, PAL_I2C_EVENT_SUCCESS);
}

void i2c_master_end_of_receive_callback(void) {

    i2c_result_t i2_result;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    i2_result.i2c_ctx = gp_pal_i2c_current_ctx;
    i2_result.i2c_result = PAL_I2C_EVENT_SUCCESS;

    /*
     * You cann't call callback from the timer callback, this might lead to a corruption
     * Use queues instead to activate corresponding handler
     * */
    xQueueSendFromISR(trustx_i2cresult_queue, (void *)&i2_result, &xHigherPriorityTaskWoken);
    //invoke_upper_layer_callback(gp_pal_i2c_current_ctx, PAL_I2C_EVENT_SUCCESS);
}

void i2c_master_error_detected_callback(void) {

    I2C_TypeDef *p_i2c_master;
    i2c_result_t i2_result;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    p_i2c_master = gp_pal_i2c_current_ctx->p_i2c_hw_config;

    if (I2C_IntGet(p_i2c_master) & I2C_IF_TXC) {

        I2C_IntDisable(p_i2c_master, I2C_IF_TXC);
        I2C_Reset(p_i2c_master);
    }

    // Abortar la recepción si está ocupada
    if (I2C_IntGet(p_i2c_master) & I2C_IF_RXDATAV) {

        I2C_IntDisable(p_i2c_master, I2C_IF_RXDATAV);
        I2C_Reset(p_i2c_master);
    }
    i2_result.i2c_ctx = gp_pal_i2c_current_ctx;
    i2_result.i2c_result = PAL_I2C_EVENT_ERROR;

    xQueueSendFromISR(trustx_i2cresult_queue, (void *)&i2_result, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    //invoke_upper_layer_callback(gp_pal_i2c_current_ctx, PAL_I2C_EVENT_ERROR);
}

void i2c_master_nack_received_callback(void) {
    i2c_master_error_detected_callback();
}

void i2c_master_arbitration_lost_callback(void) {
    i2c_master_error_detected_callback();
}

void i2c_result_handler(void *pvParameters) {

    i2c_result_t i2_result;

    do {
        if (xQueueReceive(trustx_i2cresult_queue, &(i2_result), (TickType_t)portMAX_DELAY)) {
            invoke_upper_layer_callback(i2_result.i2c_ctx, i2_result.i2c_result);
        }
    } while (1);

    vTaskDelete(NULL);
}

pal_status_t pal_i2c_init(const pal_i2c_t *p_i2c_context) {

    #ifdef EFR32
    #else
    if (p_i2c_context == NULL || p_i2c_context->p_i2c_hw_config == NULL) {
            return PAL_STATUS_FAILURE;
        }
    #endif

    // Enable clock for I2C
    // CMU_ClockEnable(cmuClock_I2C0, true);

    // Initialize I2C
    I2CSPM_Init_TypeDef i2cInit = {
        .port = I2C0,
        .sclPort = gpioPortA,
        .sdaPort = gpioPortA,
        .sclPin = 8,
        .sdaPin = 7,
        .i2cRefFreq = 0,
        .i2cMaxFreq = I2C_FREQ_STANDARD_MAX,
        .i2cClhr = i2cClockHLRStandard
      };

      I2CSPM_Init(&i2cInit);

    return PAL_STATUS_SUCCESS;
}

pal_status_t pal_i2c_deinit(const pal_i2c_t *p_i2c_context) {

    #ifdef EFR32
    return PAL_STATUS_SUCCESS;
    #else
    if (p_i2c_context == NULL || p_i2c_context->p_i2c_hw_config == NULL) {
            return PAL_STATUS_FAILURE;
        }
    return PAL_STATUS_SUCCESS;
    #endif

    /*
    // Disable I2C peripheral before resetting it
    I2C_Enable(p_i2c_context->p_i2c_hw_config, false);

    // Reset I2C peripheral to its default state
    I2C_Reset(p_i2c_context->p_i2c_hw_config);

    // Disable clock for I2C to save power
    CMU_ClockEnable(cmuClock_I2C0, false); // Assuming cmuClock_I2C0 is the correct clock for the I2C instance

    */
}


pal_status_t pal_i2c_write(const pal_i2c_t *p_i2c_context, uint8_t *p_data, uint16_t length) {

    I2C_TransferSeq_TypeDef seq;
    I2C_TransferReturn_TypeDef result;

    pal_i2c_t *config;

    #ifdef EFR32
    pal_i2c_t local_config = {
      .p_i2c_hw_config = I2C0,
      .p_upper_layer_ctx = NULL,
      .upper_layer_event_handler = NULL,
      .slave_address = OPTIGA_ADDR
    };

    config = &local_config;
    #else
    config = p_i2c_context;
    #endif

    if (config == NULL) {
        return PAL_STATUS_FAILURE;
    }
    // Initialize I2C transfer sequence
    seq.addr = config->slave_address << 1; // Shift the address for write operation
    seq.flags = I2C_FLAG_WRITE; // Indicate a write operation
    // Setup the buffer for the sequence
    seq.buf[0].data = p_data;
    seq.buf[0].len = length;

    result = I2CSPM_Transfer(config->p_i2c_hw_config, &seq);
      if (result != i2cTransferDone) {
        return PAL_STATUS_FAILURE;
      }
    return PAL_STATUS_SUCCESS;

}

pal_status_t pal_i2c_read(const pal_i2c_t *p_i2c_context, uint8_t *p_data, uint16_t length) {

    I2C_TransferSeq_TypeDef seq;
    I2C_TransferReturn_TypeDef result;

    const pal_i2c_t *config;

    #ifdef EFR32
    pal_i2c_t local_config = {
        .p_i2c_hw_config = I2C0,
        .p_upper_layer_ctx = NULL,
        .upper_layer_event_handler = NULL,
        .slave_address = OPTIGA_ADDR
    };
    config = &local_config;
    #else
    config = p_i2c_context;
    #endif

    if (config == NULL) {
        return PAL_STATUS_FAILURE;
    }

    // Acquire the I2C bus before read/write
    if (PAL_STATUS_SUCCESS == pal_i2c_acquire(config)) {
        gp_pal_i2c_current_ctx = config;

        // Setup I2C transfer for reading
        seq.addr = (config->slave_address << 1) | 1; // Shift the address and set read bit
        seq.flags = I2C_FLAG_READ; // Indicate a read operation
        // Setup the buffer for the sequence
        seq.buf[0].data = p_data;
        seq.buf[0].len = length;

        result = I2CSPM_Transfer(config->p_i2c_hw_config, &seq);
        if (result == i2cTransferDone) {
            return PAL_STATUS_SUCCESS;
        }
    }

    return PAL_STATUS_FAILURE;


    /*
        // Start I2C transfer
        result = I2C_TransferInit(p_i2c_context->p_i2c_hw_config, &seq);

        // Process the transfer result
        while (result == i2cTransferInProgress) {
            result = I2C_Transfer(p_i2c_context->p_i2c_hw_config);
        }

        if (result == i2cTransferDone) {
            // Transfer completed successfully
            status = PAL_STATUS_SUCCESS;
            // Invoke upper layer callback with success
            ((upper_layer_callback_t)(p_i2c_context->upper_layer_event_handler))(p_i2c_context->p_upper_layer_ctx, PAL_I2C_EVENT_SUCCESS);
        } else {
            // If I2C Master fails to invoke the read operation, invoke upper layer event handler with error.
            ((upper_layer_callback_t)(p_i2c_context->upper_layer_event_handler))(p_i2c_context->p_upper_layer_ctx, PAL_I2C_EVENT_ERROR);
        }

        // Release I2C Bus
        pal_i2c_release((void *)p_i2c_context);
    } else {
        status = PAL_STATUS_I2C_BUSY;
        // Invoke upper layer event handler with busy status
        ((upper_layer_callback_t)(p_i2c_context->upper_layer_event_handler))(p_i2c_context->p_upper_layer_ctx, PAL_I2C_EVENT_BUSY);
    }*/
}

// Function to set the I2C bitrate
static pal_status_t foo_i2c_set_baudrate(uint16_t bitrate) {
    I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT; // Default settings
    CMU_Clock_TypeDef i2cClock;
    uint32_t i2cFreq;
    pal_status_t status = PAL_STATUS_FAILURE;

    // Assuming I2C0 is used, adjust as necessary
    i2cClock = cmuClock_I2C0;
    i2cFreq = CMU_ClockFreqGet(i2cClock);

    // Calculate the I2C clock divider based on the desired bitrate and the I2C clock frequency
    // This is a simplified approach; refer to the MCU's reference manual for accurate calculation
    uint32_t clkDiv = (i2cFreq + (bitrate * 4) - 1) / (bitrate * 4);
    if (clkDiv > 256) { // Assuming 256 is the max clkDiv supported
        clkDiv = 256;
    }

    // Configure the I2C with the calculated clock divider
    i2cInit.freq = i2cFreq / clkDiv;
    I2C_Init(I2C0, &i2cInit); // Adjust I2C0 as necessary

    status = PAL_STATUS_SUCCESS;
    return status;
}

// Modified pal_i2c_set_bitrate to use foo_i2c_set_baudrate
pal_status_t pal_i2c_set_bitrate(const pal_i2c_t *p_i2c_context, uint16_t bitrate) {
    pal_status_t return_status = PAL_STATUS_FAILURE;
    optiga_lib_status_t event = PAL_I2C_EVENT_ERROR;

    if (PAL_STATUS_SUCCESS == pal_i2c_acquire(p_i2c_context)) {
        if (bitrate > PAL_I2C_MASTER_MAX_BITRATE) {
            bitrate = PAL_I2C_MASTER_MAX_BITRATE;
        }

        if (PAL_STATUS_SUCCESS == foo_i2c_set_baudrate(bitrate)) {
            return_status = PAL_STATUS_SUCCESS;
            event = PAL_I2C_EVENT_SUCCESS;
        } else {
            return_status = PAL_STATUS_FAILURE;
            event = PAL_I2C_EVENT_ERROR;
        }

        if (0 != p_i2c_context->upper_layer_event_handler) {
            ((callback_handler_t)(p_i2c_context->upper_layer_event_handler))(p_i2c_context->p_upper_layer_ctx, event);
        }

        if (PAL_STATUS_I2C_BUSY != return_status) {
            pal_i2c_release((void *)p_i2c_context);
        }
    } else {
        return_status = PAL_STATUS_I2C_BUSY;
        event = PAL_I2C_EVENT_BUSY;
    }

    return return_status;
}

/**
 * @}
 */
