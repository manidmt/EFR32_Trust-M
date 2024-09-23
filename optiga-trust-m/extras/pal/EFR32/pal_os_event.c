/**
 * SPDX-FileCopyrightText: 2019-2024 Infineon Technologies AG
 * SPDX-License-Identifier: MIT
 *
 * \author Infineon Technologies AG
 *
 * \file pal_os_event.c
 *
 * \brief   This file implements the platform abstraction layer APIs for os event/scheduler.
 *
 * \ingroup  grPAL
 *
 * @{
 */



#include "pal_os_event.h"
#include "FreeRTOS.h"
#include "task.h"


static volatile register_callback callback_registered = NULL;

static pal_os_event_t pal_os_event_0 = {0};
static uint32_t pal_os_ts_0 = 0;

void pal_os_event_start(
    pal_os_event_t *p_pal_os_event,
    register_callback callback,
    void *callback_args
) {
    if (FALSE == p_pal_os_event->is_event_triggered) {
        p_pal_os_event->is_event_triggered = TRUE;
        pal_os_event_register_callback_oneshot(p_pal_os_event, callback, callback_args, 1000);
    }
}

void pal_os_event_stop(pal_os_event_t *p_pal_os_event) {
    // lint --e{714} suppress "The API pal_os_event_stop is not exposed in header file but used as extern in
    // optiga_cmd.c"
    p_pal_os_event->is_event_triggered = FALSE;
}

pal_os_event_t *pal_os_event_create(register_callback callback, void *callback_args) {
    if ((NULL != callback) && (NULL != callback_args)) {
        pal_os_event_start(&pal_os_event_0, callback, callback_args);
    }
    return (&pal_os_event_0);
}

void pal_os_event_trigger_registered_callback(void) {
    register_callback callback;

#ifdef CHANGES_&_PRINTING
    printf("pal_os_event_trigger_registered_callback: Verificando, tiempo actual = %u, tiempo registrado = %u\n", xTaskGetTickCount(), pal_os_ts_0);
#endif

    if ((pal_os_ts_0 != 0) && (pal_os_ts_0 < xTaskGetTickCount())
        && pal_os_event_0.callback_registered) {
#ifdef CHANGES_&_PRINTING
        printf("pal_os_event_trigger_registered_callback: Disparando callback registrado, contexto = %p\n", pal_os_event_0.callback_ctx);
#endif
        pal_os_ts_0 = 0;
        callback = pal_os_event_0.callback_registered;
        callback((void *)pal_os_event_0.callback_ctx);
    }
}

void pal_os_event_register_callback_oneshot(
    pal_os_event_t *p_pal_os_event,
    register_callback callback,
    void *callback_args,
    uint32_t time_us
) {
#ifdef CHANGES_&_PRINTING
    printf("pal_os_event_register_callback_oneshot: Registrando callback, contexto = %p, tiempo = %u us\n", callback_args, time_us);
#endif
    pal_os_event_0.callback_registered = callback;
    pal_os_event_0.callback_ctx = callback_args;


    pal_os_ts_0 = xTaskGetTickCount() + pdMS_TO_TICKS(time_us / 1000);
#ifdef CHANGES_&_PRINTING
    uint32_t ticks = pdMS_TO_TICKS(time_us / 1000);
    pal_os_ts_0 = xTaskGetTickCount() + (ticks > 0 ? ticks : 1);
    printf("pal_os_event_register_callback_oneshot: Tiempo del sistema actual = %lu, Tiempo registrado = %lu\n", xTaskGetTickCount(), pal_os_ts_0);
#endif
}

/**
 * @}
 */

// lint --e{818,715} suppress "As there is no implementation, pal_os_event is not used"
void pal_os_event_destroy(pal_os_event_t *pal_os_event) {}

/**
 * @}
 */
