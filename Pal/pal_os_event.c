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
#include <unistd.h>
#include "sl_sleeptimer.h"

static pal_os_event_t pal_os_event_0 = {0};

void pal_os_event_start(
    pal_os_event_t *p_pal_os_event,
    register_callback callback,
    void *callback_args
) {
    if (0 == p_pal_os_event->is_event_triggered) {
        p_pal_os_event->is_event_triggered = TRUE;
        pal_os_event_register_callback_oneshot(p_pal_os_event, callback, callback_args, 1000);
    }
}

void pal_os_event_stop(pal_os_event_t *p_pal_os_event) {
    p_pal_os_event->is_event_triggered = 0;
}

pal_os_event_t *pal_os_event_create(register_callback callback, void *callback_args) {
    if ((NULL != callback) && (NULL != callback_args)) {
        pal_os_event_start(&pal_os_event_0, callback, callback_args);
    }
    return (&pal_os_event_0);
}

void pal_os_event_trigger_registered_callback(void) {
    register_callback callback;

    // !!!OPTIGA_LIB_PORTING_REQUIRED
    // User should take care to stop the timer if it sin't stoped automatically
    // IMPORTANT: Make sure you don't call this callback from the ISR.
    // It could work, but not recommended.

    if (pal_os_event_0.callback_registered) {
        callback = pal_os_event_0.callback_registered;
        callback((void *)pal_os_event_0.callback_ctx);
    }
}

static void sleeptimer_callback(sl_sleeptimer_timer_handle_t *handle, void *data) {
    (void)handle;
    pal_os_event_trigger_registered_callback();
}

void pal_os_event_register_callback_oneshot(
    pal_os_event_t *p_pal_os_event,
    register_callback callback,
    void *callback_args,
    uint32_t time_us
) {
    p_pal_os_event->callback_registered = callback;
    p_pal_os_event->callback_ctx = callback_args;

    /*
    pthread_t timer_thread;
        struct timer_params {
            pal_os_event_t *event;
            uint32_t time_us;
        } *params = malloc(sizeof(struct timer_params));

        params->event = p_pal_os_event;
        params->time_us = time_us;

        void *timer_thread_func(void *args) {
            struct timer_params *params = (struct timer_params *)args;
            usleep(params->time_us);
            pal_os_event_trigger_registered_callback();
            free(params);
            return NULL;
        }

    pthread_create(&timer_thread, NULL, timer_thread_func, params);
    pthread_detach(timer_thread);*/

    p_pal_os_event->callback_registered = callback;
    p_pal_os_event->callback_ctx = callback_args;

    // Convertir tiempo en microsegundos a milisegundos
    uint32_t time_ms = (time_us + 999) / 1000; // Redondear hacia arriba


    sl_sleeptimer_timer_handle_t timer_handle;
    sl_status_t status = sl_sleeptimer_start_timer(&timer_handle,
                                                   time_ms,
                                                   sleeptimer_callback,
                                                   p_pal_os_event,
                                                   0,
                                                   0);

    if (status != SL_STATUS_OK) {

    }
}

void pal_os_event_destroy(pal_os_event_t *pal_os_event) {
    (void)pal_os_event;
    // User should take care to destroy the event if it's not required
}

/**
 * @}
 */
