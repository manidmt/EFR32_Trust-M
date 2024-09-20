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

#include "app.h"
#include "FreeRTOS.h"
#include "task.h"
#include "pal_os_event.h"

void vEventCheckTask(void *pvParameters) {
    while (1) {
        pal_os_event_trigger_registered_callback();
        vTaskDelay(pdMS_TO_TICKS(10));  // Llama a la verificación cada 10 ms
    }
}


void app_init(void) {
    // Inicializa otras tareas o recursos necesarios

    // Crea la tarea de verificación de eventos
    if (xTaskCreate(vEventCheckTask, "EventCheck", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        // Manejar el error de creación de tarea
        printf("Error al crear la tarea de verificación de eventos\n");
    }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
}
