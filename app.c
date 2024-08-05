/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
#include <stdio.h>
#include <time.h>
#include "../Pal/pal_i2c.h"
int contador = 0;

void my_sleep(unsigned duration)
{
    time_t start = time(NULL);
    double end = duration;
    time_t now;
    do {
        now = time(NULL);
    } while (difftime(now, start) < end);
}


void app_init(void)
{
  /*
  pal_i2c_t config = {
    .p_i2c_hw_config = I2C0,
    .p_upper_layer_ctx = NULL,
    .upper_layer_event_handler = NULL,
    .slave_address = OPTIGA_ADDR
  };*/
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if (contador%2==0)    printf("\na");
  else                  printf("\nb");
  //my_sleep(500);
  contador++;
}
