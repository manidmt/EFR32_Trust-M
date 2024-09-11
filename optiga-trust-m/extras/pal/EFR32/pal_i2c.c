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

    if (p_i2c_context == NULL || p_i2c_context->p_i2c_hw_config == NULL) {
          return PAL_STATUS_FAILURE;
    }

    uint16_t status = PAL_STATUS_FAILURE;

    if (xIicCallbackTaskHandle == NULL) {

        xIicSemaphoreHandle = xSemaphoreCreateBinary();

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

        status = PAL_STATUS_SUCCESS;

        xTaskCreate(
            i2c_result_handler,
            "TrstI2CXHndlr",
            configMINIMAL_STACK_SIZE,
            NULL,
            tskIDLE_PRIORITY,
            &xIicCallbackTaskHandle
        );


        trustx_i2cresult_queue = xQueueCreate(2, sizeof(i2c_result_t));


        pal_i2c_release(p_i2c_context);
    } else {

        status = PAL_STATUS_SUCCESS;
    }

    return status;
}

pal_status_t pal_i2c_deinit(const pal_i2c_t *p_i2c_context) {


    if (xIicCallbackTaskHandle != NULL)
          vTaskDelete(xIicCallbackTaskHandle);

    pal_i2c_acquire(p_i2c_context);

    vSemaphoreDelete(xIicSemaphoreHandle);

    return PAL_STATUS_SUCCESS;
    /*
    #ifdef EFR32
    return PAL_STATUS_SUCCESS;
    #else
    if (p_i2c_context == NULL || p_i2c_context->p_i2c_hw_config == NULL) {
            return PAL_STATUS_FAILURE;
        }
    return PAL_STATUS_SUCCESS;
    #endif


    // Disable I2C peripheral before resetting it
    I2C_Enable(p_i2c_context->p_i2c_hw_config, false);

    // Reset I2C peripheral to its default state
    I2C_Reset(p_i2c_context->p_i2c_hw_config);

    // Disable clock for I2C to save power
    CMU_ClockEnable(cmuClock_I2C0, false); // Assuming cmuClock_I2C0 is the correct clock for the I2C instance

    */
}


pal_status_t pal_i2c_write(const pal_i2c_t *p_i2c_context, uint8_t *p_data, uint16_t length) {
    pal_status_t status = PAL_STATUS_FAILURE;

    // Acquire the I2C bus before read/write
    if (PAL_STATUS_SUCCESS == pal_i2c_acquire(p_i2c_context)) {
        gp_pal_i2c_current_ctx = p_i2c_context;
        /*
        const pal_i2c_t *config;
        pal_i2c_t local_config = {
            .p_i2c_hw_config = I2C0,
            .p_upper_layer_ctx = NULL,
            .upper_layer_event_handler = NULL,
            .slave_address = OPTIGA_ADDR
        };
        config = &local_config;
        */

        // Initialize I2C transfer sequence
        I2C_TransferSeq_TypeDef seq;
        seq.addr = p_i2c_context->slave_address << 1; // Shift the address for write operation
        seq.flags = I2C_FLAG_WRITE; // Indicate a write operation
        seq.buf[0].data = p_data;
        seq.buf[0].len = length;

        // Invoke the low-level I2C master driver API to write to the bus
        I2C_TransferReturn_TypeDef result = I2CSPM_Transfer(p_i2c_context->p_i2c_hw_config, &seq);

        if (result != i2cTransferDone) {
            // If I2C Master fails to invoke the write operation, invoke upper layer event handler with error.
            ((upper_layer_callback_t)(p_i2c_context->upper_layer_event_handler)
            )(p_i2c_context->p_upper_layer_ctx, PAL_I2C_EVENT_ERROR);

            // Release I2C Bus
            pal_i2c_release(p_i2c_context);
        } else {
            status = PAL_STATUS_SUCCESS;
        }
    } else {
        status = PAL_STATUS_I2C_BUSY;
        ((upper_layer_callback_t)(p_i2c_context->upper_layer_event_handler)
        )(p_i2c_context->p_upper_layer_ctx, PAL_I2C_EVENT_BUSY);
    }
    return status;
}

pal_status_t pal_i2c_read(const pal_i2c_t *p_i2c_context, uint8_t *p_data, uint16_t length) {
    pal_status_t status = PAL_STATUS_FAILURE;

    // Acquire the I2C bus before read/write
    if (PAL_STATUS_SUCCESS == pal_i2c_acquire(p_i2c_context)) {
        gp_pal_i2c_current_ctx = p_i2c_context;

        /*
        const pal_i2c_t *config;
        pal_i2c_t local_config = {
            .p_i2c_hw_config = I2C0,
            .p_upper_layer_ctx = NULL,
            .upper_layer_event_handler = NULL,
            .slave_address = OPTIGA_ADDR
        };
        config = &local_config;
        */

        // Initialize I2C transfer sequence for reading
        I2C_TransferSeq_TypeDef seq;
        seq.addr = p_i2c_context->slave_address << 1; // Shift the address for read operation
        seq.flags = I2C_FLAG_READ; // Indicate a read operation
        seq.buf[0].data = p_data;
        seq.buf[0].len = length;

        // Invoke the low-level I2C master driver API to read from the bus
        I2C_TransferReturn_TypeDef result = I2CSPM_Transfer(p_i2c_context->p_i2c_hw_config, &seq);

        if (result != i2cTransferDone) {
            // If I2C Master fails to invoke the read operation, invoke upper layer event handler with error.
            ((upper_layer_callback_t)(p_i2c_context->upper_layer_event_handler)
            )(p_i2c_context->p_upper_layer_ctx, PAL_I2C_EVENT_ERROR);

            // Release I2C Bus
            pal_i2c_release(p_i2c_context);
        } else {
            status = PAL_STATUS_SUCCESS;
            // Informa el éxito a la capa superior
            ((upper_layer_callback_t)(p_i2c_context->upper_layer_event_handler)
            )(p_i2c_context->p_upper_layer_ctx, PAL_I2C_EVENT_SUCCESS);
        }
    } else {
        status = PAL_STATUS_I2C_BUSY;
        ((upper_layer_callback_t)(p_i2c_context->upper_layer_event_handler)
        )(p_i2c_context->p_upper_layer_ctx, PAL_I2C_EVENT_BUSY);
    }
    return status;


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

/*
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
}*/

// Modified pal_i2c_set_bitrate to use foo_i2c_set_baudrate
pal_status_t pal_i2c_set_bitrate(const pal_i2c_t *p_i2c_context, uint16_t bitrate) {

    pal_status_t return_status = PAL_STATUS_FAILURE;
    optiga_lib_status_t event = PAL_I2C_EVENT_ERROR;

    // Acquire the I2C bus before setting the bitrate
    if (PAL_STATUS_SUCCESS == pal_i2c_acquire(p_i2c_context)) {
        // If the user-provided bitrate is greater than the I2C master hardware maximum supported value,
        // set the I2C master to its maximum supported value.
        if (bitrate > PAL_I2C_MASTER_MAX_BITRATE) {
            bitrate = PAL_I2C_MASTER_MAX_BITRATE;
        }

        // Convert bitrate to frequency in Hz
        uint32_t freq = bitrate * 1000;

        // Set the bitrate for the I2C peripheral
        I2C_BusFreqSet(p_i2c_context->p_i2c_hw_config, 0, freq, i2cClockHLRStandard);

        if (I2C_BusFreqGet(p_i2c_context->p_i2c_hw_config) == freq) {
            return_status = PAL_STATUS_SUCCESS;
            event = PAL_I2C_EVENT_SUCCESS;
        } else {
            return_status = PAL_STATUS_FAILURE;
        }


    } else {
        return_status = PAL_STATUS_I2C_BUSY;
        event = PAL_I2C_EVENT_BUSY;
    }

    // Notify the upper layer handler
    if (p_i2c_context->upper_layer_event_handler) {
        ((callback_handler_t)(p_i2c_context->upper_layer_event_handler)
        )(p_i2c_context->p_upper_layer_ctx, event);
    }

    // Release I2C Bus

    pal_i2c_release((void *)p_i2c_context);
    return return_status;

  /*
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

    return return_status;*/
}

/**
 * @}
 */
