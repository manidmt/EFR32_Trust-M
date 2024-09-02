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
#include "em_gpio.h"
#include "em_cmu.h"

pal_status_t pal_gpio_init(const pal_gpio_t *p_gpio_context)
{
    if (p_gpio_context == NULL || p_gpio_context->p_gpio_hw == NULL)
    {
        return PAL_STATUS_FAILURE;
    }

    efr32_gpio_context_t *gpio_hw = (efr32_gpio_context_t *)p_gpio_context->p_gpio_hw;

    CMU_ClockEnable(cmuClock_GPIO, true);

    GPIO_PinModeSet(gpio_hw->port, gpio_hw->pin, gpioModePushPull, 0);

    return PAL_STATUS_SUCCESS;
}

pal_status_t pal_gpio_deinit(const pal_gpio_t *p_gpio_context) {

  if (p_gpio_context == NULL || p_gpio_context->p_gpio_hw == NULL)
  {
      return PAL_STATUS_FAILURE;
  }
  efr32_gpio_context_t *gpio_hw = (efr32_gpio_context_t *)p_gpio_context->p_gpio_hw;

  GPIO_PinModeSet(gpio_hw->port, gpio_hw->pin, gpioModeDisabled, 0)

  return PAL_STATUS_SUCCESS;
}

void pal_gpio_set_high(const pal_gpio_t *p_gpio_context) {

  if (p_gpio_context == NULL || p_gpio_context->p_gpio_hw == NULL)
  {
      return;
  }

  efr32_gpio_context_t *gpio_hw = (efr32_gpio_context_t *)p_gpio_context->p_gpio_hw;
  GPIO_PinOutSet(gpio_hw->port, gpio_hw->pin);
}

void pal_gpio_set_low(const pal_gpio_t *p_gpio_context) {
  if (p_gpio_context == NULL || p_gpio_context->p_gpio_hw == NULL)
      {
          return;
      }

      efr32_gpio_context_t *gpio_hw = (efr32_gpio_context_t *)p_gpio_context->p_gpio_hw;
      GPIO_PinOutClear(gpio_hw->port, gpio_hw->pin);
}

/**
 * @}
 */
