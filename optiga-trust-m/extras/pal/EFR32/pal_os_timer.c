/**
 * SPDX-FileCopyrightText: 2019-2024 Infineon Technologies AG
 * SPDX-License-Identifier: MIT
 *
 * \author Infineon Technologies AG
 *
 * \file pal_os_timer.c
 *
 * \brief   This file implements the platform abstraction layer APIs for timer.
 *
 * \ingroup  grPAL
 *
 * @{
 */

//#include "em_cmu.h"
//#include "em_timer.h"
#include "pal_os_timer.h"

#include "FreeRTOS.h"
#include "stdio.h"
#include "task.h"

//pal_status_t pal_os_timer_init(void)
//{
//    CMU_ClockEnable(cmuClock_TIMER0, true);
//
//    TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;
//    timerInit.prescale = timerPrescale1;
//    TIMER_Init(TIMER0, &timerInit);
//
//    return PAL_STATUS_SUCCESS;
//}

uint32_t pal_os_timer_get_time_in_microseconds(void)
{
    return xTaskGetTickCount() * 1000;
}

uint32_t pal_os_timer_get_time_in_milliseconds(void)
{
    //return TIMER_CounterGet(TIMER0) / 1000;
    return xTaskGetTickCount();
}

void pal_os_timer_delay_in_milliseconds(uint16_t milliseconds)
{
    const TickType_t xDelay = milliseconds / portTICK_PERIOD_MS;
    vTaskDelay(xDelay);
//    uint32_t start_time = pal_os_timer_get_time_in_milliseconds();
//    while ((pal_os_timer_get_time_in_milliseconds() - start_time) < milliseconds)
//    {}
}

//pal_status_t pal_os_timer_deinit(void)
//{
//    CMU_ClockEnable(cmuClock_TIMER0, false);
//    return PAL_STATUS_SUCCESS;
//}

/**
 * @}
 */
