################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include simplicity_sdk_2024.6.0/platform/service/system/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/memory_manager/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/iostream/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/interrupt_manager/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/device_manager/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/device_manager/clocks/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/device_init/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/clock_manager/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/emlib/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/emdrv/dmadrv/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/driver/debug/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/common/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/Device/SiliconLabs/EFR32FG23/Source/subdir.mk
-include simplicity_sdk_2024.6.0/hardware/driver/configuration_over_swo/src/subdir.mk
-include simplicity_sdk_2024.6.0/hardware/board/src/subdir.mk
-include autogen/subdir.mk
-include Pal/mbedtls/subdir.mk
-include Pal/subdir.mk
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


BUILD_ARTIFACT_NAME := TrustM
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: TrustM.axf

# Tool invocations
TrustM.axf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -T "C:\Users\DiazMecoterr\SimplicityStudio\v5_workspace\TrustM\autogen\linkerfile.ld" -Wl,--no-warn-rwx-segments -Xlinker --gc-sections -Xlinker -Map="TrustM.map" -mfpu=fpv5-sp-d16 -mfloat-abi=hard --specs=nano.specs -o TrustM.axf -Wl,--start-group "./Pal/example_optiga_init_deinit.o" "./Pal/example_utilities.o" "./Pal/ifx_i2c.o" "./Pal/ifx_i2c_config.o" "./Pal/ifx_i2c_data_link_layer.o" "./Pal/ifx_i2c_physical_layer.o" "./Pal/ifx_i2c_presentation_layer.o" "./Pal/ifx_i2c_transport_layer.o" "./Pal/optiga_cmd.o" "./Pal/optiga_comms_ifx_i2c.o" "./Pal/optiga_crypt.o" "./Pal/optiga_lib_common.o" "./Pal/optiga_lib_logger.o" "./Pal/optiga_util.o" "./Pal/pal.o" "./Pal/pal_crypt_mbedtls.o" "./Pal/pal_gpio.o" "./Pal/pal_i2c.o" "./Pal/pal_ifx_i2c_config.o" "./Pal/pal_logger.o" "./Pal/pal_os_datastore.o" "./Pal/pal_os_event.o" "./Pal/pal_os_lock_efr32.o" "./Pal/pal_os_timer.o" "./Pal/mbedtls/aes.o" "./Pal/mbedtls/aesni.o" "./Pal/mbedtls/arc4.o" "./Pal/mbedtls/aria.o" "./Pal/mbedtls/asn1parse.o" "./Pal/mbedtls/asn1write.o" "./Pal/mbedtls/base64.o" "./Pal/mbedtls/bignum.o" "./Pal/mbedtls/blowfish.o" "./Pal/mbedtls/camellia.o" "./Pal/mbedtls/ccm.o" "./Pal/mbedtls/certs.o" "./Pal/mbedtls/chacha20.o" "./Pal/mbedtls/chachapoly.o" "./Pal/mbedtls/cipher.o" "./Pal/mbedtls/cipher_wrap.o" "./Pal/mbedtls/cmac.o" "./Pal/mbedtls/ctr_drbg.o" "./Pal/mbedtls/debug.o" "./Pal/mbedtls/des.o" "./Pal/mbedtls/dhm.o" "./Pal/mbedtls/ecdh.o" "./Pal/mbedtls/ecdsa.o" "./Pal/mbedtls/ecjpake.o" "./Pal/mbedtls/ecp.o" "./Pal/mbedtls/ecp_curves.o" "./Pal/mbedtls/entropy.o" "./Pal/mbedtls/entropy_poll.o" "./Pal/mbedtls/error.o" "./Pal/mbedtls/gcm.o" "./Pal/mbedtls/havege.o" "./Pal/mbedtls/hkdf.o" "./Pal/mbedtls/hmac_drbg.o" "./Pal/mbedtls/md.o" "./Pal/mbedtls/md2.o" "./Pal/mbedtls/md4.o" "./Pal/mbedtls/md5.o" "./Pal/mbedtls/md_wrap.o" "./Pal/mbedtls/memory_buffer_alloc.o" "./Pal/mbedtls/net_sockets.o" "./Pal/mbedtls/nist_kw.o" "./Pal/mbedtls/oid.o" "./Pal/mbedtls/padlock.o" "./Pal/mbedtls/pem.o" "./Pal/mbedtls/pk.o" "./Pal/mbedtls/pk_wrap.o" "./Pal/mbedtls/pkcs11.o" "./Pal/mbedtls/pkcs12.o" "./Pal/mbedtls/pkcs5.o" "./Pal/mbedtls/pkparse.o" "./Pal/mbedtls/pkwrite.o" "./Pal/mbedtls/platform.o" "./Pal/mbedtls/platform_util.o" "./Pal/mbedtls/poly1305.o" "./Pal/mbedtls/ripemd160.o" "./Pal/mbedtls/rsa.o" "./Pal/mbedtls/rsa_internal.o" "./Pal/mbedtls/sha1.o" "./Pal/mbedtls/sha256.o" "./Pal/mbedtls/sha512.o" "./Pal/mbedtls/ssl_cache.o" "./Pal/mbedtls/ssl_ciphersuites.o" "./Pal/mbedtls/ssl_cli.o" "./Pal/mbedtls/ssl_cookie.o" "./Pal/mbedtls/ssl_srv.o" "./Pal/mbedtls/ssl_ticket.o" "./Pal/mbedtls/ssl_tls.o" "./Pal/mbedtls/threading.o" "./Pal/mbedtls/timing.o" "./Pal/mbedtls/version.o" "./Pal/mbedtls/version_features.o" "./Pal/mbedtls/x509.o" "./Pal/mbedtls/x509_create.o" "./Pal/mbedtls/x509_crl.o" "./Pal/mbedtls/x509_crt.o" "./Pal/mbedtls/x509_csr.o" "./Pal/mbedtls/x509write_crt.o" "./Pal/mbedtls/x509write_csr.o" "./Pal/mbedtls/xtea.o" "./app.o" "./main.o" "./autogen/sl_board_default_init.o" "./autogen/sl_event_handler.o" "./autogen/sl_iostream_handles.o" "./autogen/sl_iostream_init_usart_instances.o" "./simplicity_sdk_2024.6.0/hardware/board/src/sl_board_control_gpio.o" "./simplicity_sdk_2024.6.0/hardware/board/src/sl_board_init.o" "./simplicity_sdk_2024.6.0/hardware/driver/configuration_over_swo/src/sl_cos.o" "./simplicity_sdk_2024.6.0/platform/Device/SiliconLabs/EFR32FG23/Source/startup_efr32fg23.o" "./simplicity_sdk_2024.6.0/platform/Device/SiliconLabs/EFR32FG23/Source/system_efr32fg23.o" "./simplicity_sdk_2024.6.0/platform/common/src/sl_assert.o" "./simplicity_sdk_2024.6.0/platform/common/src/sl_core_cortexm.o" "./simplicity_sdk_2024.6.0/platform/common/src/sl_string.o" "./simplicity_sdk_2024.6.0/platform/common/src/sl_syscalls.o" "./simplicity_sdk_2024.6.0/platform/driver/debug/src/sl_debug_swo.o" "./simplicity_sdk_2024.6.0/platform/emdrv/dmadrv/src/dmadrv.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_cmu.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_core.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_emu.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_gpio.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_i2c.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_ldma.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_msc.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_system.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_usart.o" "./simplicity_sdk_2024.6.0/platform/service/clock_manager/src/sl_clock_manager.o" "./simplicity_sdk_2024.6.0/platform/service/clock_manager/src/sl_clock_manager_hal_s2.o" "./simplicity_sdk_2024.6.0/platform/service/clock_manager/src/sl_clock_manager_init.o" "./simplicity_sdk_2024.6.0/platform/service/clock_manager/src/sl_clock_manager_init_hal_s2.o" "./simplicity_sdk_2024.6.0/platform/service/device_init/src/sl_device_init_dcdc_s2.o" "./simplicity_sdk_2024.6.0/platform/service/device_init/src/sl_device_init_emu_s2.o" "./simplicity_sdk_2024.6.0/platform/service/device_manager/clocks/sl_device_clock_efr32xg23.o" "./simplicity_sdk_2024.6.0/platform/service/device_manager/src/sl_device_clock.o" "./simplicity_sdk_2024.6.0/platform/service/interrupt_manager/src/sl_interrupt_manager_cortexm.o" "./simplicity_sdk_2024.6.0/platform/service/iostream/src/sl_iostream.o" "./simplicity_sdk_2024.6.0/platform/service/iostream/src/sl_iostream_retarget_stdio.o" "./simplicity_sdk_2024.6.0/platform/service/iostream/src/sl_iostream_uart.o" "./simplicity_sdk_2024.6.0/platform/service/iostream/src/sl_iostream_usart.o" "./simplicity_sdk_2024.6.0/platform/service/memory_manager/src/sl_memory_manager_region.o" "./simplicity_sdk_2024.6.0/platform/service/system/src/sl_system_init.o" "./simplicity_sdk_2024.6.0/platform/service/system/src/sl_system_process_action.o" -lgcc -lc -lm -lnosys -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building out file: TrustM.out'
	arm-none-eabi-objcopy "TrustM.axf" "TrustM.out"
	@echo ' '

	@echo 'Building hex file: TrustM.hex'
	arm-none-eabi-objcopy -O ihex "TrustM.axf" "TrustM.hex"
	@echo ' '

	@echo 'Building bin file: TrustM.bin'
	arm-none-eabi-objcopy -O binary "TrustM.axf" "TrustM.bin"
	@echo ' '

	@echo 'Building s37 file: TrustM.s37'
	arm-none-eabi-objcopy -O srec "TrustM.axf" "TrustM.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "TrustM.axf" -A
	@echo ' '

# Other Targets
clean:
	-@$(foreach FILE, $(EXECUTABLES), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(OBJS), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(C_DEPS), $(shell $(RM) $(FILE)) )
	-$(RM) TrustM.axf
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
