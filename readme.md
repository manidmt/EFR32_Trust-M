# EFR32 drivers for Optiga Trut M

Project involving the creation of drivers between the EFR32 and Optiga Trust M using I/O Stream over USART and FreeRTOS


It gets stuck in the `ifx_i2c_init` function probably due to a mis-passing of the arguments required for the correct use of my implementation of `pal_gpio_set_low` found in the `pal_gpio.c` file.

Several macros have been added to make it easier to understand the flow of the program and some tests that I have been doing to check that everything was working correctly. If these macros are removed the program will continue to work correctly and the additional debugging messages will not appear.
