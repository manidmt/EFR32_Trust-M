/**
 * SPDX-FileCopyrightText: 2021-2024 Infineon Technologies AG
 * SPDX-License-Identifier: MIT
 *
 * \author Infineon Technologies AG
 *
 * \file ifx_i2c.c
 *
 * \brief   This file implements the wrapper API Layer for IFX I2C protocol v2.00.
 *
 * \ingroup  grIFXI2C
 *
 * @{
 */

#include "ifx_i2c.h"

#include "pal_os_event.h"

#ifndef OPTIGA_COMMS_SHIELDED_CONNECTION
#include "ifx_i2c_transport_layer.h"
#else
#include "ifx_i2c_presentation_layer.h"
#endif

/// @cond hidden

// IFX I2C states
#define IFX_I2C_STATE_UNINIT (0x01)
#define IFX_I2C_STATE_IDLE (0x02)
#define IFX_I2C_STATUS_BUSY (0x03)
#define IFX_I2C_STATUS_NOT_BUSY (0x04)

/// IFX I2C Reset states
#define IFX_I2C_STATE_RESET_PIN_LOW (0xB1)
#define IFX_I2C_STATE_RESET_PIN_HIGH (0xB2)
#define IFX_I2C_STATE_RESET_INIT (0xB3)

void ifx_i2c_tl_event_handler(
    ifx_i2c_context_t *p_ctx,
    optiga_lib_status_t event,
    const uint8_t *p_data,
    uint16_t data_len
);

#ifdef OPTIGA_COMMS_SHIELDED_CONNECTION
_STATIC_H void ifx_i2c_prl_close_event_handler(
    ifx_i2c_context_t *p_ctx,
    optiga_lib_status_t event,
    const uint8_t *p_data,
    uint16_t data_len
);
#endif
_STATIC_H optiga_lib_status_t ifx_i2c_init(ifx_i2c_context_t *p_ifx_i2c_context);

// lint --e{526} suppress "This API is defined in ifx_i2c_physical_layer.c file. As it is a low level API, it is not exposed in header file"
extern optiga_lib_status_t ifx_i2c_pl_write_slave_address(
    ifx_i2c_context_t *p_ctx,
    uint8_t slave_address,
    uint8_t storage_type
);
/// @endcond

optiga_lib_status_t ifx_i2c_open(ifx_i2c_context_t *p_ctx) {
    optiga_lib_status_t api_status = (int32_t)IFX_I2C_STACK_ERROR;
#ifdef DEPURATION_BY_PRINTING
    printf("ifx_i2c_open: Initializing I2C opening\n");
#endif

    // If api status is not busy, proceed
    if ((IFX_I2C_STATUS_BUSY != p_ctx->status)) {
        do {
            p_ctx->p_pal_i2c_ctx->p_upper_layer_ctx = p_ctx;
            p_ctx->reset_type = OPTIGA_COMMS_DEFAULT_RESET_TYPE;
            if (p_ctx->reset_type > (uint8_t)IFX_I2C_WARM_RESET) {
                break;
            }
            p_ctx->reset_state = IFX_I2C_STATE_RESET_PIN_LOW;
            p_ctx->do_pal_init = TRUE;
            p_ctx->state = IFX_I2C_STATE_UNINIT;
#ifdef  DEPURATION_BY_PRINTING
            printf("ifx_i2c_open: State after initializing: %d\n", p_ctx->state);
            printf("ifx_i2c_open: Calling ifx_i2c_init\n");
#endif
            api_status = ifx_i2c_init(p_ctx);
#ifdef  DEPURATION_BY_PRINTING
            printf("ifx_i2c_open: Status after ifx_i2c_init: %d\n", api_status);
#endif
            if (IFX_I2C_STACK_SUCCESS == api_status) {
                p_ctx->status = IFX_I2C_STATUS_BUSY;
#ifdef  DEPURATION_BY_PRINTING
                printf("ifx_i2c_open: I2C status set to BUSY\n");
#endif
            }
        } while (FALSE);
    }
    return (api_status);
}

optiga_lib_status_t ifx_i2c_reset(ifx_i2c_context_t *p_ctx, ifx_i2c_reset_type_t reset_type) {
    optiga_lib_status_t api_status = (int32_t)IFX_I2C_STACK_ERROR;

    // Proceed, if not busy and in idle state
    if ((IFX_I2C_STATE_IDLE == p_ctx->state) && (IFX_I2C_STATUS_BUSY != p_ctx->status)) {
        p_ctx->reset_type = (uint8_t)reset_type;
        p_ctx->reset_state = IFX_I2C_STATE_RESET_PIN_LOW;
        p_ctx->do_pal_init = FALSE;

        api_status = ifx_i2c_init(p_ctx);
        if (IFX_I2C_STACK_SUCCESS == api_status) {
            p_ctx->status = IFX_I2C_STATUS_BUSY;
        }
    }
    return (api_status);
}

optiga_lib_status_t ifx_i2c_transceive(
    ifx_i2c_context_t *p_ctx,
    const uint8_t *p_tx_data,
    uint16_t tx_data_length,
    uint8_t *p_rx_buffer,
    uint16_t *p_rx_buffer_len
) {
    optiga_lib_status_t api_status = (int32_t)IFX_I2C_STACK_ERROR;
    // Proceed, if not busy and in idle state
    if ((IFX_I2C_STATE_IDLE == p_ctx->state) && (IFX_I2C_STATUS_BUSY != p_ctx->status)) {
        p_ctx->p_upper_layer_rx_buffer = p_rx_buffer;
        p_ctx->p_upper_layer_rx_buffer_len = p_rx_buffer_len;
#ifndef OPTIGA_COMMS_SHIELDED_CONNECTION
        api_status = ifx_i2c_tl_transceive(
            p_ctx,
            (uint8_t *)p_tx_data,
            (tx_data_length),
            (uint8_t *)p_rx_buffer,
            p_rx_buffer_len
        );
        if (IFX_I2C_STACK_SUCCESS == api_status) {
            p_ctx->status = IFX_I2C_STATUS_BUSY;
        }
#else
        api_status = ifx_i2c_prl_transceive(
            p_ctx,
            (uint8_t *)p_tx_data,
            tx_data_length,
            (uint8_t *)p_rx_buffer,
            p_rx_buffer_len
        );
#endif
        if ((IFX_I2C_STACK_SUCCESS == api_status)
            && (IFX_I2C_STACK_SUCCESS == p_ctx->close_state)) {
            p_ctx->status = IFX_I2C_STATUS_BUSY;
        }
    }
    return (api_status);
}

optiga_lib_status_t ifx_i2c_close(ifx_i2c_context_t *p_ctx) {
    optiga_lib_status_t api_status = (int32_t)IFX_I2C_STACK_ERROR;
    // Proceed, if not busy and in idle state
    if (IFX_I2C_STATUS_BUSY != p_ctx->status) {
        api_status = IFX_I2C_STACK_SUCCESS;

#ifdef OPTIGA_COMMS_SHIELDED_CONNECTION
        p_ctx->close_state = IFX_I2C_STACK_ERROR;
        p_ctx->state = IFX_I2C_STATE_UNINIT;
        // lint --e{838} suppress "Previous value of api_status is needed for scenario when shielded connection is disabled"
        api_status = ifx_i2c_prl_close(p_ctx, ifx_i2c_prl_close_event_handler);
        if (IFX_I2C_STACK_ERROR == api_status) {
            // lint --e{534} suppress "Error handling is not required so return value is not checked"
            pal_i2c_deinit(p_ctx->p_pal_i2c_ctx);
            // Also power off the device
            pal_gpio_set_low(p_ctx->p_slave_vdd_pin);
            pal_gpio_set_low(p_ctx->p_slave_reset_pin);
            p_ctx->status = IFX_I2C_STATUS_NOT_BUSY;
        }
#else
        ifx_i2c_tl_event_handler(p_ctx, IFX_I2C_STACK_SUCCESS, NULL, 0);
        // lint --e{534} suppress "Error handling is not required so return value is not checked"
        //  Close I2C master
        pal_i2c_deinit(p_ctx->p_pal_i2c_ctx);
        // Also power off the device
        pal_gpio_set_low(p_ctx->p_slave_vdd_pin);
        pal_gpio_set_low(p_ctx->p_slave_reset_pin);
        p_ctx->state = IFX_I2C_STATE_UNINIT;
        p_ctx->status = IFX_I2C_STATUS_NOT_BUSY;
#endif
    }
    return (api_status);
}

optiga_lib_status_t
ifx_i2c_set_slave_address(ifx_i2c_context_t *p_ctx, uint8_t slave_address, uint8_t persistent) {
    optiga_lib_status_t api_status = (int32_t)IFX_I2C_STACK_ERROR;

    if ((IFX_I2C_STATE_IDLE == p_ctx->state)) {
        p_ctx->p_pal_i2c_ctx->p_upper_layer_ctx = p_ctx;

        api_status = ifx_i2c_pl_write_slave_address(p_ctx, slave_address, persistent);
    }

    return (api_status);
}

/// @cond hidden
// lint --e{715} suppress "The arguments p_data and data_len is not used in this function
//                         but as per the function signature those 2 parameter should be passed"
void ifx_i2c_tl_event_handler(
    ifx_i2c_context_t *p_ctx,
    optiga_lib_status_t event,
    const uint8_t *p_data,
    uint16_t data_len
) {
    // If there is no upper layer handler, don't do anything and return
    if (NULL != p_ctx->upper_layer_event_handler) {
        p_ctx->upper_layer_event_handler(p_ctx->p_upper_layer_ctx, event);
    }
    p_ctx->close_state = event;
    p_ctx->status = IFX_I2C_STATUS_NOT_BUSY;
    switch (p_ctx->state) {
        case IFX_I2C_STATE_UNINIT: {
            if (IFX_I2C_STACK_SUCCESS == event) {
                p_ctx->state = IFX_I2C_STATE_IDLE;
            }
            break;
        }
        default:
            break;
    }
}
#ifdef OPTIGA_COMMS_SHIELDED_CONNECTION
/// @cond hidden
// lint --e{715} suppress "The arguments p_data and data_len is not used in this function
//                         but as per the function signature those 2 parameter should be passed"
_STATIC_H void ifx_i2c_prl_close_event_handler(
    ifx_i2c_context_t *p_ctx,
    optiga_lib_status_t event,
    const uint8_t *p_data,
    uint16_t data_len
) {
    p_ctx->status = IFX_I2C_STATUS_NOT_BUSY;
    switch (p_ctx->state) {
        case IFX_I2C_STATE_UNINIT: {
            // lint --e{534} suppress "Error handling is not required so return value is not checked"
            pal_i2c_deinit(p_ctx->p_pal_i2c_ctx);
            // Also power off the device
            pal_gpio_set_low(p_ctx->p_slave_vdd_pin);
            pal_gpio_set_low(p_ctx->p_slave_reset_pin);
            break;
        }
        default:
            break;
    }
    // If there is no upper layer handler, don't do anything and return
    if (NULL != p_ctx->upper_layer_event_handler) {
        p_ctx->upper_layer_event_handler(p_ctx->p_upper_layer_ctx, event);
    }
}
#endif
_STATIC_H optiga_lib_status_t ifx_i2c_init(ifx_i2c_context_t *p_ifx_i2c_context) {
    optiga_lib_status_t api_status = IFX_I2C_STACK_ERROR;
#ifdef  DEPURATION_BY_PRINTING
    printf("ifx_i2c_init: Iniciando inicialización de I2C, reset_type = %d, reset_state = %d\n", p_ifx_i2c_context->reset_type, p_ifx_i2c_context->reset_state);
#endif
#ifdef CHANGES
    efr32_gpio_context_t reset_pin_hw = {
        .port = gpioPortA,
        .pin = 0
    };
    pal_gpio_t reset_pin = {
        .p_gpio_hw = &reset_pin_hw
    };
    p_ifx_i2c_context->p_slave_reset_pin = &reset_pin;

#endif

    if (((uint8_t)IFX_I2C_WARM_RESET == p_ifx_i2c_context->reset_type)
        || ((uint8_t)IFX_I2C_COLD_RESET == p_ifx_i2c_context->reset_type)) {
        switch (p_ifx_i2c_context->reset_state) {
            case IFX_I2C_STATE_RESET_PIN_LOW: {
#ifdef  DEPURATION_BY_PRINTING
                printf("ifx_i2c_init: Estado IFX_I2C_STATE_RESET_PIN_LOW\n");

                const pal_gpio_t *reset_pin = p_ifx_i2c_context->p_slave_reset_pin;

                // Verificar si reset_pin y reset_pin->p_gpio_hw son válidos
                if (reset_pin == NULL || reset_pin->p_gpio_hw == NULL) {
                    printf("ifx_i2c_init: reset_pin o p_gpio_hw es NULL\n");
                } else {
                    printf("ifx_i2c_init: reset_pin = %p, reset_pin->p_gpio_hw = %p\n", reset_pin, reset_pin->p_gpio_hw);
                    efr32_gpio_context_t *gpio_hw = (efr32_gpio_context_t *)(reset_pin->p_gpio_hw);
                    printf("ifx_i2c_init: reset_pin contexto valido, Port = %d, Pin = %d\n", gpio_hw->port, gpio_hw->pin);
                }

                const pal_gpio_t *vdd_pin = p_ifx_i2c_context->p_slave_vdd_pin;

                if (vdd_pin == NULL || vdd_pin->p_gpio_hw == NULL) {
                    printf("ifx_i2c_init: vdd_pin o p_gpio_hw es NULL\n");
                } else {
                    printf("ifx_i2c_init: vdd_pin = %p, vdd_pin->p_gpio_hw = %p\n", vdd_pin, vdd_pin->p_gpio_hw);
                    efr32_gpio_context_t *gpio_hw_vdd = (efr32_gpio_context_t *)(vdd_pin->p_gpio_hw);
                    printf("ifx_i2c_init: vdd_pin contexto válido, Port = %d, Pin = %d\n", gpio_hw_vdd->port, gpio_hw_vdd->pin);
                }
#endif
                // Setting the Vdd & Reset pin to low
                if ((uint8_t)IFX_I2C_COLD_RESET == p_ifx_i2c_context->reset_type) {
                    pal_gpio_set_low(p_ifx_i2c_context->p_slave_vdd_pin);
                }
                pal_gpio_set_low(p_ifx_i2c_context->p_slave_reset_pin);
                p_ifx_i2c_context->reset_state = IFX_I2C_STATE_RESET_PIN_HIGH;
                pal_os_event_register_callback_oneshot(
                    p_ifx_i2c_context->pal_os_event_ctx,
                    (register_callback)ifx_i2c_init,
                    (void *)p_ifx_i2c_context,
                    RESET_LOW_TIME_MSEC
                );
#ifdef  DEPURATION_BY_PRINTING
                printf("ifx_i2c_init: Registrado callback para IFX_I2C_STATE_RESET_PIN_HIGH\n");
#endif
                api_status = IFX_I2C_STACK_SUCCESS;
                break;
            }
            case IFX_I2C_STATE_RESET_PIN_HIGH: {
#ifdef  DEPURATION_BY_PRINTING
                printf("ifx_i2c_init: Estado IFX_I2C_STATE_RESET_PIN_HIGH\n");
#endif
                // Setting the Vdd & Reset pin to high
                if ((uint8_t)IFX_I2C_COLD_RESET == p_ifx_i2c_context->reset_type) {
                    pal_gpio_set_high(p_ifx_i2c_context->p_slave_vdd_pin);
                }
                pal_gpio_set_high(p_ifx_i2c_context->p_slave_reset_pin);
                p_ifx_i2c_context->reset_state = IFX_I2C_STATE_RESET_INIT;
                pal_os_event_register_callback_oneshot(
                    p_ifx_i2c_context->pal_os_event_ctx,
                    (register_callback)ifx_i2c_init,
                    (void *)p_ifx_i2c_context,
                    STARTUP_TIME_MSEC
                );
#ifdef  DEPURATION_BY_PRINTING
                printf("ifx_i2c_init: Registrado callback para IFX_I2C_STATE_RESET_INIT\n");
#endif
                api_status = IFX_I2C_STACK_SUCCESS;
                break;
            }
            case IFX_I2C_STATE_RESET_INIT: {
#ifdef  DEPURATION_BY_PRINTING
                printf("ifx_i2c_init: Estado IFX_I2C_STATE_RESET_INIT\n");
#endif
                // Frequency and frame size negotiation
#ifndef OPTIGA_COMMS_SHIELDED_CONNECTION
                api_status = ifx_i2c_tl_init(p_ifx_i2c_context, ifx_i2c_tl_event_handler);
#else
                api_status = ifx_i2c_prl_init(p_ifx_i2c_context, ifx_i2c_tl_event_handler);
#endif
                break;
            }
            default:
#ifdef  DEPURATION_BY_PRINTING
                printf("ifx_i2c_init: Estado desconocido\n");
#endif
                break;
        }
    }
    // soft reset
    else {
#ifdef  DEPURATION_BY_PRINTING
        printf("ifx_i2c_init: Estado Soft Reset\n");
#endif
        p_ifx_i2c_context->pl.request_soft_reset = (uint8_t)TRUE;  // Soft reset
#ifndef OPTIGA_COMMS_SHIELDED_CONNECTION
        api_status = ifx_i2c_tl_init(p_ifx_i2c_context, ifx_i2c_tl_event_handler);
#else
        api_status = ifx_i2c_prl_init(p_ifx_i2c_context, ifx_i2c_tl_event_handler);
#endif
    }
    if (api_status != IFX_I2C_STACK_SUCCESS) {
        ifx_i2c_tl_event_handler(p_ifx_i2c_context, api_status, 0, 0);
    }
#ifdef  DEPURATION_BY_PRINTING
    printf("ifx_i2c_init: Finalización de ifx_i2c_init, api_status = %d\n", api_status);
#endif
    return (api_status);
}
/// @endcond
/**
 * @}
 */
