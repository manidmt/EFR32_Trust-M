################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/portable/MemMang/subdir.mk
-include simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/portable/GCC/ARM_CM33_NTZ/non_secure/subdir.mk
-include simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/subdir.mk
-include simplicity_sdk_2024.6.0/util/third_party/freertos/cmsis/Source/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/udelay/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/system/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/sleeptimer/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/memory_manager/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/iostream/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/interrupt_manager/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/device_manager/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/device_manager/devices/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/device_manager/clocks/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/device_init/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/clock_manager/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/peripheral/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/emlib/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/emdrv/dmadrv/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/driver/i2cspm/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/driver/debug/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/common/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/Device/SiliconLabs/EFR32FG23/Source/subdir.mk
-include simplicity_sdk_2024.6.0/platform/CMSIS/RTOS2/Source/subdir.mk
-include simplicity_sdk_2024.6.0/hardware/driver/configuration_over_swo/src/subdir.mk
-include simplicity_sdk_2024.6.0/hardware/board/src/subdir.mk
-include optiga-trust-m/src/util/subdir.mk
-include optiga-trust-m/src/crypt/subdir.mk
-include optiga-trust-m/src/comms/ifx_i2c/subdir.mk
-include optiga-trust-m/src/comms/subdir.mk
-include optiga-trust-m/src/common/subdir.mk
-include optiga-trust-m/src/cmd/subdir.mk
-include optiga-trust-m/mbedtls/subdir.mk
-include optiga-trust-m/extras/pal/EFR32/subdir.mk
-include optiga-trust-m/extras/pal/subdir.mk
-include autogen/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Optiga
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: Optiga.axf

# Tool invocations
Optiga.axf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -T "C:\Users\DiazMecoterr\SimplicityStudio\v5_workspace\Optiga\autogen\linkerfile.ld" -Wl,--wrap=_free_r -Wl,--wrap=_malloc_r -Wl,--wrap=_calloc_r -Wl,--wrap=_realloc_r -Wl,--no-warn-rwx-segments -Xlinker --gc-sections -Xlinker -Map="Optiga.map" -mfpu=fpv5-sp-d16 -mfloat-abi=hard --specs=nano.specs -o Optiga.axf -Wl,--start-group "./app.o" "./main.o" "./autogen/sl_board_default_init.o" "./autogen/sl_event_handler.o" "./autogen/sl_i2cspm_init.o" "./autogen/sl_iostream_handles.o" "./optiga-trust-m/extras/pal/EFR32/pal.o" "./optiga-trust-m/extras/pal/EFR32/pal_gpio.o" "./optiga-trust-m/extras/pal/EFR32/pal_i2c.o" "./optiga-trust-m/extras/pal/EFR32/pal_ifx_i2c_config.o" "./optiga-trust-m/extras/pal/EFR32/pal_logger.o" "./optiga-trust-m/extras/pal/EFR32/pal_os_datastore.o" "./optiga-trust-m/extras/pal/EFR32/pal_os_event.o" "./optiga-trust-m/extras/pal/EFR32/pal_os_lock.o" "./optiga-trust-m/extras/pal/EFR32/pal_os_memory.o" "./optiga-trust-m/extras/pal/EFR32/pal_os_timer.o" "./optiga-trust-m/extras/pal/pal_crypt_mbedtls.o" "./optiga-trust-m/mbedtls/aes.o" "./optiga-trust-m/mbedtls/aesni.o" "./optiga-trust-m/mbedtls/arc4.o" "./optiga-trust-m/mbedtls/aria.o" "./optiga-trust-m/mbedtls/asn1parse.o" "./optiga-trust-m/mbedtls/asn1write.o" "./optiga-trust-m/mbedtls/base64.o" "./optiga-trust-m/mbedtls/bignum.o" "./optiga-trust-m/mbedtls/blowfish.o" "./optiga-trust-m/mbedtls/camellia.o" "./optiga-trust-m/mbedtls/ccm.o" "./optiga-trust-m/mbedtls/certs.o" "./optiga-trust-m/mbedtls/chacha20.o" "./optiga-trust-m/mbedtls/chachapoly.o" "./optiga-trust-m/mbedtls/cipher.o" "./optiga-trust-m/mbedtls/cipher_wrap.o" "./optiga-trust-m/mbedtls/cmac.o" "./optiga-trust-m/mbedtls/ctr_drbg.o" "./optiga-trust-m/mbedtls/debug.o" "./optiga-trust-m/mbedtls/des.o" "./optiga-trust-m/mbedtls/dhm.o" "./optiga-trust-m/mbedtls/ecdh.o" "./optiga-trust-m/mbedtls/ecdsa.o" "./optiga-trust-m/mbedtls/ecjpake.o" "./optiga-trust-m/mbedtls/ecp.o" "./optiga-trust-m/mbedtls/ecp_curves.o" "./optiga-trust-m/mbedtls/entropy.o" "./optiga-trust-m/mbedtls/entropy_poll.o" "./optiga-trust-m/mbedtls/error.o" "./optiga-trust-m/mbedtls/gcm.o" "./optiga-trust-m/mbedtls/havege.o" "./optiga-trust-m/mbedtls/hkdf.o" "./optiga-trust-m/mbedtls/hmac_drbg.o" "./optiga-trust-m/mbedtls/md.o" "./optiga-trust-m/mbedtls/md2.o" "./optiga-trust-m/mbedtls/md4.o" "./optiga-trust-m/mbedtls/md5.o" "./optiga-trust-m/mbedtls/md_wrap.o" "./optiga-trust-m/mbedtls/memory_buffer_alloc.o" "./optiga-trust-m/mbedtls/net_sockets.o" "./optiga-trust-m/mbedtls/nist_kw.o" "./optiga-trust-m/mbedtls/oid.o" "./optiga-trust-m/mbedtls/padlock.o" "./optiga-trust-m/mbedtls/pem.o" "./optiga-trust-m/mbedtls/pk.o" "./optiga-trust-m/mbedtls/pk_wrap.o" "./optiga-trust-m/mbedtls/pkcs11.o" "./optiga-trust-m/mbedtls/pkcs12.o" "./optiga-trust-m/mbedtls/pkcs5.o" "./optiga-trust-m/mbedtls/pkparse.o" "./optiga-trust-m/mbedtls/pkwrite.o" "./optiga-trust-m/mbedtls/platform.o" "./optiga-trust-m/mbedtls/platform_util.o" "./optiga-trust-m/mbedtls/poly1305.o" "./optiga-trust-m/mbedtls/ripemd160.o" "./optiga-trust-m/mbedtls/rsa.o" "./optiga-trust-m/mbedtls/rsa_internal.o" "./optiga-trust-m/mbedtls/sha1.o" "./optiga-trust-m/mbedtls/sha256.o" "./optiga-trust-m/mbedtls/sha512.o" "./optiga-trust-m/mbedtls/ssl_cache.o" "./optiga-trust-m/mbedtls/ssl_ciphersuites.o" "./optiga-trust-m/mbedtls/ssl_cli.o" "./optiga-trust-m/mbedtls/ssl_cookie.o" "./optiga-trust-m/mbedtls/ssl_srv.o" "./optiga-trust-m/mbedtls/ssl_ticket.o" "./optiga-trust-m/mbedtls/ssl_tls.o" "./optiga-trust-m/mbedtls/threading.o" "./optiga-trust-m/mbedtls/timing.o" "./optiga-trust-m/mbedtls/version.o" "./optiga-trust-m/mbedtls/version_features.o" "./optiga-trust-m/mbedtls/x509.o" "./optiga-trust-m/mbedtls/x509_create.o" "./optiga-trust-m/mbedtls/x509_crl.o" "./optiga-trust-m/mbedtls/x509_crt.o" "./optiga-trust-m/mbedtls/x509_csr.o" "./optiga-trust-m/mbedtls/x509write_crt.o" "./optiga-trust-m/mbedtls/x509write_csr.o" "./optiga-trust-m/mbedtls/xtea.o" "./optiga-trust-m/src/cmd/optiga_cmd.o" "./optiga-trust-m/src/common/optiga_lib_common.o" "./optiga-trust-m/src/common/optiga_lib_logger.o" "./optiga-trust-m/src/comms/ifx_i2c/ifx_i2c.o" "./optiga-trust-m/src/comms/ifx_i2c/ifx_i2c_config.o" "./optiga-trust-m/src/comms/ifx_i2c/ifx_i2c_data_link_layer.o" "./optiga-trust-m/src/comms/ifx_i2c/ifx_i2c_physical_layer.o" "./optiga-trust-m/src/comms/ifx_i2c/ifx_i2c_presentation_layer.o" "./optiga-trust-m/src/comms/ifx_i2c/ifx_i2c_transport_layer.o" "./optiga-trust-m/src/comms/optiga_comms_ifx_i2c.o" "./optiga-trust-m/src/crypt/optiga_crypt.o" "./optiga-trust-m/src/util/optiga_util.o" "./simplicity_sdk_2024.6.0/hardware/board/src/sl_board_control_gpio.o" "./simplicity_sdk_2024.6.0/hardware/board/src/sl_board_init.o" "./simplicity_sdk_2024.6.0/hardware/driver/configuration_over_swo/src/sl_cos.o" "./simplicity_sdk_2024.6.0/platform/CMSIS/RTOS2/Source/os_systick.o" "./simplicity_sdk_2024.6.0/platform/Device/SiliconLabs/EFR32FG23/Source/startup_efr32fg23.o" "./simplicity_sdk_2024.6.0/platform/Device/SiliconLabs/EFR32FG23/Source/system_efr32fg23.o" "./simplicity_sdk_2024.6.0/platform/common/src/sl_assert.o" "./simplicity_sdk_2024.6.0/platform/common/src/sl_core_cortexm.o" "./simplicity_sdk_2024.6.0/platform/common/src/sl_string.o" "./simplicity_sdk_2024.6.0/platform/common/src/sl_syscalls.o" "./simplicity_sdk_2024.6.0/platform/common/src/sli_cmsis_os2_ext_task_register.o" "./simplicity_sdk_2024.6.0/platform/driver/debug/src/sl_debug_swo.o" "./simplicity_sdk_2024.6.0/platform/driver/i2cspm/src/sl_i2cspm.o" "./simplicity_sdk_2024.6.0/platform/emdrv/dmadrv/src/dmadrv.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_burtc.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_cmu.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_core.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_emu.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_gpio.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_i2c.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_ldma.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_msc.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_prs.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_system.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_timer.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_usart.o" "./simplicity_sdk_2024.6.0/platform/peripheral/src/sl_hal_sysrtc.o" "./simplicity_sdk_2024.6.0/platform/service/clock_manager/src/sl_clock_manager.o" "./simplicity_sdk_2024.6.0/platform/service/clock_manager/src/sl_clock_manager_hal_s2.o" "./simplicity_sdk_2024.6.0/platform/service/clock_manager/src/sl_clock_manager_init.o" "./simplicity_sdk_2024.6.0/platform/service/clock_manager/src/sl_clock_manager_init_hal_s2.o" "./simplicity_sdk_2024.6.0/platform/service/device_init/src/sl_device_init_dcdc_s2.o" "./simplicity_sdk_2024.6.0/platform/service/device_init/src/sl_device_init_emu_s2.o" "./simplicity_sdk_2024.6.0/platform/service/device_manager/clocks/sl_device_clock_efr32xg23.o" "./simplicity_sdk_2024.6.0/platform/service/device_manager/devices/sl_device_peripheral_hal_efr32xg23.o" "./simplicity_sdk_2024.6.0/platform/service/device_manager/src/sl_device_clock.o" "./simplicity_sdk_2024.6.0/platform/service/device_manager/src/sl_device_peripheral.o" "./simplicity_sdk_2024.6.0/platform/service/interrupt_manager/src/sl_interrupt_manager_cortexm.o" "./simplicity_sdk_2024.6.0/platform/service/iostream/src/sl_iostream.o" "./simplicity_sdk_2024.6.0/platform/service/iostream/src/sl_iostream_retarget_stdio.o" "./simplicity_sdk_2024.6.0/platform/service/iostream/src/sl_iostream_stdlib_config.o" "./simplicity_sdk_2024.6.0/platform/service/iostream/src/sl_iostream_uart.o" "./simplicity_sdk_2024.6.0/platform/service/iostream/src/sl_iostream_usart.o" "./simplicity_sdk_2024.6.0/platform/service/memory_manager/src/sl_memory_manager.o" "./simplicity_sdk_2024.6.0/platform/service/memory_manager/src/sl_memory_manager_dynamic_reservation.o" "./simplicity_sdk_2024.6.0/platform/service/memory_manager/src/sl_memory_manager_pool.o" "./simplicity_sdk_2024.6.0/platform/service/memory_manager/src/sl_memory_manager_region.o" "./simplicity_sdk_2024.6.0/platform/service/memory_manager/src/sl_memory_manager_retarget.o" "./simplicity_sdk_2024.6.0/platform/service/memory_manager/src/sli_memory_manager_common.o" "./simplicity_sdk_2024.6.0/platform/service/sleeptimer/src/sl_sleeptimer.o" "./simplicity_sdk_2024.6.0/platform/service/sleeptimer/src/sl_sleeptimer_hal_burtc.o" "./simplicity_sdk_2024.6.0/platform/service/sleeptimer/src/sl_sleeptimer_hal_sysrtc.o" "./simplicity_sdk_2024.6.0/platform/service/sleeptimer/src/sl_sleeptimer_hal_timer.o" "./simplicity_sdk_2024.6.0/platform/service/system/src/sl_system_init.o" "./simplicity_sdk_2024.6.0/platform/service/system/src/sl_system_kernel.o" "./simplicity_sdk_2024.6.0/platform/service/udelay/src/sl_udelay.o" "./simplicity_sdk_2024.6.0/platform/service/udelay/src/sl_udelay_armv6m_gcc.o" "./simplicity_sdk_2024.6.0/util/third_party/freertos/cmsis/Source/cmsis_os2.o" "./simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/croutine.o" "./simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/event_groups.o" "./simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/list.o" "./simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/queue.o" "./simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/stream_buffer.o" "./simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/tasks.o" "./simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/timers.o" "./simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/portable/GCC/ARM_CM33_NTZ/non_secure/port.o" "./simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.o" "./simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/portable/MemMang/heap_3.o" -lgcc -lc -lm -lnosys -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building out file: Optiga.out'
	arm-none-eabi-objcopy "Optiga.axf" "Optiga.out"
	@echo ' '

	@echo 'Building hex file: Optiga.hex'
	arm-none-eabi-objcopy -O ihex "Optiga.axf" "Optiga.hex"
	@echo ' '

	@echo 'Building bin file: Optiga.bin'
	arm-none-eabi-objcopy -O binary "Optiga.axf" "Optiga.bin"
	@echo ' '

	@echo 'Building s37 file: Optiga.s37'
	arm-none-eabi-objcopy -O srec "Optiga.axf" "Optiga.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "Optiga.axf" -A
	@echo ' '

# Other Targets
clean:
	-@$(foreach FILE, $(EXECUTABLES), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(OBJS), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(C_DEPS), $(shell $(RM) $(FILE)) )
	-$(RM) Optiga.axf
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
