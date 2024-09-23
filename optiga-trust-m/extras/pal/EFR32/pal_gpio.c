/**
 * SPDX-FileCopyrightText: 2019-2024 Infineon Technologies AG
 * SPDX-License-Identifier: MIT
 *
 * \author Infineon Technologies AG
 *
 * \file pal_gpio.c
 *
 * \brief   This file implements the platform abstraction layer APIs for GPIO.
 *
 * \ingroup  grPAL
 *
 * @{
 */

#include "pal_gpio.h"

pal_status_t pal_gpio_init(const pal_gpio_t *p_gpio_context)
{
    if (p_gpio_context == NULL || p_gpio_context->p_gpio_hw == NULL)
    {
#ifdef DEPURATION_BY_PRINTING
        printf("pal_gpio_init: Contexto GPIO nulo\n");
#endif
        return PAL_STATUS_FAILURE;
    }

    efr32_gpio_context_t *gpio_hw = (efr32_gpio_context_t *)p_gpio_context->p_gpio_hw;
#ifdef DEPURATION_BY_PRINTING
    printf("pal_gpio_init: Habilitando reloj del GPIO\n");
#endif
    CMU_ClockEnable(cmuClock_GPIO, true);

#ifdef DEPURATION_BY_PRINTING
    printf("pal_gpio_init: Configurando modo del pin GPIO\n");
#endif
    GPIO_PinModeSet(gpio_hw->port, gpio_hw->pin, gpioModePushPull, 0);

#ifdef DEPURATION_BY_PRINTING
    printf("pal_gpio_init: Inicialización completada\n");
#endif
    return PAL_STATUS_SUCCESS;
}

pal_status_t pal_gpio_deinit(const pal_gpio_t *p_gpio_context) {

    if (p_gpio_context == NULL || p_gpio_context->p_gpio_hw == NULL)
    {
        return PAL_STATUS_FAILURE;
    }
    efr32_gpio_context_t *gpio_hw = (efr32_gpio_context_t *)p_gpio_context->p_gpio_hw;

    GPIO_PinModeSet(gpio_hw->port, gpio_hw->pin, gpioModeDisabled, 0);

    return PAL_STATUS_SUCCESS;
}

void pal_gpio_set_high(const pal_gpio_t *p_gpio_context) {

    if (p_gpio_context == NULL || p_gpio_context->p_gpio_hw == NULL)
    {
        return;
    }

#ifdef DEPURATION_BY_PRINTING
    printf("pal_gpio_set_high: Setting pin %p high\n", p_gpio_context->p_gpio_hw);
#endif

    efr32_gpio_context_t *gpio_hw = (efr32_gpio_context_t *)p_gpio_context->p_gpio_hw;
#ifdef DEPURATION_BY_PRINTING
    printf("pal_gpio_set_low: Port = %d, Pin = %d\n", gpio_hw->port, gpio_hw->pin);
#endif
    pal_gpio_init(p_gpio_context);
    GPIO_PinOutSet(gpio_hw->port, gpio_hw->pin);
}

void pal_gpio_set_low(const pal_gpio_t *p_gpio_context) {

    if (p_gpio_context == NULL || p_gpio_context->p_gpio_hw == NULL)
    {
        return;
    }

#ifdef DEPURATION_BY_PRINTING
    printf("pal_gpio_set_low: Setting pin %p low\n", p_gpio_context->p_gpio_hw);
#endif

    efr32_gpio_context_t *gpio_hw = (efr32_gpio_context_t *)p_gpio_context->p_gpio_hw;
#ifdef DEPURATION_BY_PRINTING
    printf("pal_gpio_set_low: Port = %d, Pin = %d\n", gpio_hw->port, gpio_hw->pin);

    printf("pal_gpio_set_low: Llamando a pal_gpio_init\n");
#endif

    pal_gpio_init(p_gpio_context);
#ifdef DEPURATION_BY_PRINTING
    printf("pal_gpio_set_low: pal_gpio_init completado\n");

    printf("pal_gpio_set_low: Llamando a GPIO_PinOutClear\n");
#endif

    GPIO_PinOutClear(gpio_hw->port, gpio_hw->pin);
}

/**
 * @}
 */
