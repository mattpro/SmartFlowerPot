PROJECT_NAME     := experimental_ant_shared_channel_master_to_master_pca10040_s332
TARGETS          := nrf52832_xxaa
OUTPUT_DIRECTORY := _build

SDK_ROOT := ../../../../../../../..
PROJ_DIR := ../../..

$(OUTPUT_DIRECTORY)/nrf52832_xxaa.out: \
  LINKER_SCRIPT  := experimental_ant_shared_channel_master_to_master_gcc_nrf52.ld
# Source files common to all targets
SRC_FILES += \
  $(SDK_ROOT)/components/libraries/button/app_button.c \
  $(SDK_ROOT)/components/libraries/util/app_error.c \
  $(SDK_ROOT)/components/libraries/util/app_error_weak.c \
  $(SDK_ROOT)/components/libraries/timer/app_timer.c \
  $(SDK_ROOT)/components/libraries/util/app_util_platform.c \
  $(SDK_ROOT)/components/libraries/hardfault/nrf52/handler/hardfault_handler_gcc.c \
  $(SDK_ROOT)/components/libraries/hardfault/hardfault_implementation.c \
  $(SDK_ROOT)/components/libraries/util/nrf_assert.c \
  $(SDK_ROOT)/components/drivers_nrf/clock/nrf_drv_clock.c \
  $(SDK_ROOT)/components/drivers_nrf/common/nrf_drv_common.c \
  $(SDK_ROOT)/components/drivers_nrf/gpiote/nrf_drv_gpiote.c \
  $(SDK_ROOT)/components/ant/ant_stack_config/ant_stack_config.c \
  $(PROJ_DIR)/../../../../bsp/bsp.c \
  $(PROJ_DIR)/ant_phone_connection.c \
  $(PROJ_DIR)/asc_coordinator.c \
  $(PROJ_DIR)/asc_device_registry.c \
  $(PROJ_DIR)/asc_master.c \
  $(PROJ_DIR)/asc_master_to_master.c \
  $(PROJ_DIR)/asc_pages.c \
  $(PROJ_DIR)/ble_controllable_hub.c \
  $(PROJ_DIR)/ble_phone_connection.c \
  $(PROJ_DIR)/main.c \
  $(PROJ_DIR)/n5sk_button.c \
  $(PROJ_DIR)/n5sk_led.c \
  $(SDK_ROOT)/components/ble/common/ble_advdata.c \
  $(SDK_ROOT)/components/ble/common/ble_srv_common.c \
  $(SDK_ROOT)/components/toolchain/gcc/gcc_startup_nrf52.S \
  $(SDK_ROOT)/components/toolchain/system_nrf52.c \
  $(SDK_ROOT)/components/softdevice/common/softdevice_handler/softdevice_handler.c \

# Include folders common to all targets
INC_FOLDERS += \
  $(PROJ_DIR)/config/experimental_ant_shared_channel_master_to_master_pca10040_s332 \
  $(PROJ_DIR)/config \
  $(SDK_ROOT)/components/libraries/hardfault/nrf52 \
  $(SDK_ROOT)/components/toolchain/gcc \
  $(SDK_ROOT)/components/device \
  $(SDK_ROOT)/components/drivers_nrf/hal \
  $(PROJ_DIR)/../../../../bsp \
  $(SDK_ROOT)/components \
  $(SDK_ROOT)/components/drivers_nrf/gpiote \
  $(SDK_ROOT)/components/libraries/button \
  $(SDK_ROOT)/components/libraries/log \
  $(SDK_ROOT)/components/libraries/log/src \
  $(SDK_ROOT)/components/softdevice/s332/headers \
  $(SDK_ROOT)/components/toolchain \
  $(SDK_ROOT)/components/ble/common \
  $(SDK_ROOT)/components/drivers_nrf/clock \
  ../config \
  $(SDK_ROOT)/components/libraries/hardfault \
  $(SDK_ROOT)/components/libraries/util \
  $(SDK_ROOT)/components/drivers_nrf/common \
  $(SDK_ROOT)/components/softdevice/s332/headers/nrf52 \
  $(SDK_ROOT)/components/softdevice/common/softdevice_handler \
  $(SDK_ROOT)/components/drivers_nrf/delay \
  $(SDK_ROOT)/components/ant/ant_stack_config \
  $(SDK_ROOT)/components/toolchain/cmsis/include \
  $(SDK_ROOT)/components/libraries/timer \

# Libraries common to all targets
LIB_FILES += \

# C flags common to all targets
CFLAGS += -DNRF52
CFLAGS += -DSOFTDEVICE_PRESENT
CFLAGS += -DBOARD_PCA10040
CFLAGS += -DNRF52832
CFLAGS += -DNRF52_PAN_12
CFLAGS += -DNRF52_PAN_15
CFLAGS += -DNRF52_PAN_58
CFLAGS += -DNRF52_PAN_55
CFLAGS += -DNRF52_PAN_54
CFLAGS += -DNRF52_PAN_31
CFLAGS += -DNRF52_PAN_30
CFLAGS += -DNRF52_PAN_51
CFLAGS += -DNRF52_PAN_36
CFLAGS += -DNRF52_PAN_53
CFLAGS += -DANT_STACK_SUPPORT_REQD
CFLAGS += -DCONFIG_GPIO_AS_PINRESET
CFLAGS += -DBLE_STACK_SUPPORT_REQD
CFLAGS += -DNRF_SD_BLE_API_VERSION=3
CFLAGS += -DSWI_DISABLE0
CFLAGS += -DNRF52_PAN_20
CFLAGS += -DNRF52_PAN_64
CFLAGS += -DS332
CFLAGS += -DNRF52_PAN_62
CFLAGS += -DNRF52_PAN_63
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs
CFLAGS +=  -Wall -Werror -O3 -g3
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin --short-enums 
# generate dependency output file
CFLAGS += -MP -MD

# C++ flags common to all targets
CXXFLAGS += \

# Assembler flags common to all targets
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -DNRF52
ASMFLAGS += -DSOFTDEVICE_PRESENT
ASMFLAGS += -DBOARD_PCA10040
ASMFLAGS += -DNRF52832
ASMFLAGS += -DNRF52_PAN_12
ASMFLAGS += -DNRF52_PAN_15
ASMFLAGS += -DNRF52_PAN_58
ASMFLAGS += -DNRF52_PAN_55
ASMFLAGS += -DNRF52_PAN_54
ASMFLAGS += -DNRF52_PAN_31
ASMFLAGS += -DNRF52_PAN_30
ASMFLAGS += -DNRF52_PAN_51
ASMFLAGS += -DNRF52_PAN_36
ASMFLAGS += -DNRF52_PAN_53
ASMFLAGS += -DANT_STACK_SUPPORT_REQD
ASMFLAGS += -DCONFIG_GPIO_AS_PINRESET
ASMFLAGS += -DBLE_STACK_SUPPORT_REQD
ASMFLAGS += -DNRF_SD_BLE_API_VERSION=3
ASMFLAGS += -DSWI_DISABLE0
ASMFLAGS += -DNRF52_PAN_20
ASMFLAGS += -DNRF52_PAN_64
ASMFLAGS += -DS332
ASMFLAGS += -DNRF52_PAN_62
ASMFLAGS += -DNRF52_PAN_63

# Linker flags
LDFLAGS += -mthumb -mabi=aapcs -L $(TEMPLATE_PATH) -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# let linker to dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs -lc -lnosys







.PHONY: $(TARGETS) default all clean help flash 
# Default target - first one defined
default: nrf52832_xxaa

# Print all targets that can be built
help:
	@echo following targets are available:
	@echo 	nrf52832_xxaa

TEMPLATE_PATH := $(SDK_ROOT)/components/toolchain/gcc

include $(TEMPLATE_PATH)/Makefile.common
$(foreach target, $(TARGETS), $(call define_target, $(target)))
-include $(foreach target, $(TARGETS), $($(target)_dependencies))

# Flash the program
flash: $(OUTPUT_DIRECTORY)/nrf52832_xxaa.hex
	@echo Flashing: $<
	nrfjprog --program $< -f nrf52 --chiperase
	nrfjprog --reset -f nrf52

