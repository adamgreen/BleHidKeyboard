PROJECT_NAME := BleHidKeyboard

NRF51_SDK_PATH := ../sdk
NRF51_TOOL_PATH := ../tools
TEMPLATE_PATH = $(NRF51_SDK_PATH)/components/toolchain/gcc

# Pull in the platform correct version include to properly set the GNU_BIN_PATH for your environment.
ifeq ($(OS),Windows_NT)
include Makefile.windows
else
include Makefile.posix
endif

MK := mkdir
RM := rm -rf
CP := cp

#echo suspend
ifeq ("$(VERBOSE)","1")
NO_ECHO :=
else
NO_ECHO := @
endif

# Toolchain commands
CC              := '$(GNU_BIN_PATH)arm-none-eabi-gcc'
AS              := '$(GNU_BIN_PATH)arm-none-eabi-as'
AR              := '$(GNU_BIN_PATH)arm-none-eabi-ar' -r
LD              := '$(GNU_BIN_PATH)arm-none-eabi-ld'
NM              := '$(GNU_BIN_PATH)arm-none-eabi-nm'
OBJDUMP         := '$(GNU_BIN_PATH)arm-none-eabi-objdump'
OBJCOPY         := '$(GNU_BIN_PATH)arm-none-eabi-objcopy'
SIZE            := '$(GNU_BIN_PATH)arm-none-eabi-size'

#source common to all targets
C_SOURCE_FILES += \
$(abspath $(NRF51_SDK_PATH)/components/libraries/button/app_button.c) \
$(abspath $(NRF51_SDK_PATH)/components/libraries/util/app_error.c) \
$(abspath $(NRF51_SDK_PATH)/components/libraries/fifo/app_fifo.c) \
$(abspath $(NRF51_SDK_PATH)/components/libraries/scheduler/app_scheduler.c) \
$(abspath $(NRF51_SDK_PATH)/components/libraries/timer/app_timer.c) \
$(abspath $(NRF51_SDK_PATH)/components/libraries/timer/app_timer_appsh.c) \
$(abspath $(NRF51_SDK_PATH)/components/libraries/trace/app_trace.c) \
$(abspath $(NRF51_SDK_PATH)/components/libraries/util/nrf_assert.c) \
$(abspath $(NRF51_SDK_PATH)/components/libraries/uart/retarget.c) \
$(abspath $(NRF51_SDK_PATH)/components/libraries/pwm/app_pwm.c) \
$(abspath $(NRF51_SDK_PATH)/components/libraries/sensorsim/sensorsim.c) \
$(abspath $(NRF51_SDK_PATH)/components/libraries/uart/app_uart_fifo.c) \
$(abspath $(NRF51_SDK_PATH)/components/drivers_nrf/delay/nrf_delay.c) \
$(abspath $(NRF51_SDK_PATH)/components/drivers_nrf/common/nrf_drv_common.c) \
$(abspath $(NRF51_SDK_PATH)/components/drivers_nrf/gpiote/nrf_drv_gpiote.c) \
$(abspath $(NRF51_SDK_PATH)/components/drivers_nrf/uart/nrf_drv_uart.c) \
$(abspath $(NRF51_SDK_PATH)/components/libraries/twi/app_twi.c) \
$(abspath $(NRF51_SDK_PATH)/components/drivers_nrf/twi_master/nrf_drv_twi.c) \
$(abspath $(NRF51_SDK_PATH)/components/drivers_nrf/ppi/nrf_drv_ppi.c) \
$(abspath $(NRF51_SDK_PATH)/components/drivers_nrf/timer/nrf_drv_timer.c) \
$(abspath $(NRF51_SDK_PATH)/components/drivers_nrf/pstorage/pstorage.c) \
$(abspath $(NRF51_SDK_PATH)/examples/bsp/bsp.c) \
$(abspath $(NRF51_SDK_PATH)/examples/bsp/bsp_btn_ble.c) \
$(abspath main.c) \
$(abspath $(NRF51_SDK_PATH)/components/ble/common/ble_advdata.c) \
$(abspath $(NRF51_SDK_PATH)/components/ble/ble_advertising/ble_advertising.c) \
$(abspath $(NRF51_SDK_PATH)/components/ble/ble_services/ble_bas/ble_bas.c) \
$(abspath $(NRF51_SDK_PATH)/components/ble/common/ble_conn_params.c) \
$(abspath $(NRF51_SDK_PATH)/components/ble/ble_services/ble_dis/ble_dis.c) \
$(abspath $(NRF51_SDK_PATH)/components/ble/ble_services/ble_hids/ble_hids.c) \
$(abspath $(NRF51_SDK_PATH)/components/ble/ble_racp/ble_racp.c) \
$(abspath $(NRF51_SDK_PATH)/components/ble/common/ble_srv_common.c) \
$(abspath $(NRF51_SDK_PATH)/components/ble/device_manager/device_manager_peripheral.c) \
$(abspath $(NRF51_SDK_PATH)/components/toolchain/system_nrf51.c) \
$(abspath $(NRF51_SDK_PATH)/components/softdevice/common/softdevice_handler/softdevice_handler.c) \
$(abspath $(NRF51_SDK_PATH)/components/softdevice/common/softdevice_handler/softdevice_handler_appsh.c) \

#assembly files common to all targets
ASM_SOURCE_FILES  = $(abspath $(NRF51_SDK_PATH)/components/toolchain/gcc/gcc_startup_nrf51.s)

#includes common to all targets
INC_PATHS += -I$(abspath config)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/libraries/scheduler)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/drivers_nrf/config)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/examples/bsp)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/libraries/fifo)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/ble/ble_services/ble_hids)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/drivers_nrf/delay)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/libraries/util)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/ble/device_manager)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/drivers_nrf/uart)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/libraries/pwm)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/ble/common)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/libraries/sensorsim)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/drivers_nrf/pstorage)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/ble/ble_services/ble_dis)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/device)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/libraries/uart)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/drivers_nrf/twi_master)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/libraries/twi)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/ble/ble_services/ble_gls)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/libraries/button)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/libraries/timer)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/softdevice/s110/headers)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/drivers_nrf/gpiote)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/drivers_nrf/hal)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/drivers_nrf/timer)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/drivers_nrf/ppi)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/ble/ble_racp)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/toolchain/gcc)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/toolchain)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/drivers_nrf/common)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/ble/ble_advertising)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/libraries/trace)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/ble/ble_services/ble_bas)
INC_PATHS += -I$(abspath $(NRF51_SDK_PATH)/components/softdevice/common/softdevice_handler)

OUTPUT_DIRECTORY = _build
CONFIG_DIRECTORY = config
SDK_DIRECTORY = ../sdk
SDK_EXAMPLES_DIRECTORY = $(SDK_DIRECTORY)/examples
SDK_GLS_EXAMPLE_DIRECTORY = $(SDK_EXAMPLES_DIRECTORY)/ble_peripheral/ble_app_gls/config

SDK_ZIP = nRF51_SDK_10.0.0_dc26b5e.zip
SDK_URL = https://developer.nordicsemi.com/nRF5_SDK/nRF51_SDK_v10.x.x/$(SDK_ZIP)

# Compiler flags used to enable creation of header dependency files.
DEP_FLAGS := -MMD -MP

#flags common to all targets
CFLAGS  = -DBOARD_PCA10028
CFLAGS += -DSOFTDEVICE_PRESENT
CFLAGS += -DNRF51
CFLAGS += -DS110
CFLAGS += -DBLE_STACK_SUPPORT_REQD
CFLAGS += -DSWI_DISABLE0
CFLAGS += -DBSP_UART_SUPPORT
CFLAGS += -mcpu=cortex-m0
CFLAGS += -mthumb -mabi=aapcs --std=gnu99
CFLAGS += -Wall -Werror -O3 -g3
CFLAGS += -mfloat-abi=soft
CFLAGS += $(DEP_FLAGS)
# keep every function in separate section. This will allow linker to dump unused functions
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin --short-enums

LINKER_SCRIPT=nrf51.ld

# keep every function in separate section. This will allow linker to dump unused functions
LDFLAGS += -Xlinker -Map=$(OUTPUT_DIRECTORY)/$(PROJECT_NAME).map
LDFLAGS += -mthumb -mabi=aapcs -L $(TEMPLATE_PATH) -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m0
# let linker to dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs -lc -lnosys

# Assembler flags
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -DBOARD_PCA10028
ASMFLAGS += -DSOFTDEVICE_PRESENT
ASMFLAGS += -DNRF51
ASMFLAGS += -DS110
ASMFLAGS += -DBLE_STACK_SUPPORT_REQD
ASMFLAGS += -DSWI_DISABLE0
ASMFLAGS += -DBSP_UART_SUPPORT

C_SOURCE_FILE_NAMES = $(notdir $(C_SOURCE_FILES))
C_PATHS = $(sort $(dir $(C_SOURCE_FILES) ) )
C_OBJECTS = $(addprefix $(OUTPUT_DIRECTORY)/, $(C_SOURCE_FILE_NAMES:.c=.o) )

ASM_SOURCE_FILE_NAMES = $(notdir $(ASM_SOURCE_FILES))
ASM_PATHS = $(sort $(dir $(ASM_SOURCE_FILES) ))
ASM_OBJECTS = $(addprefix $(OUTPUT_DIRECTORY)/, $(ASM_SOURCE_FILE_NAMES:.s=.o) )

vpath %.c $(C_PATHS)
vpath %.s $(ASM_PATHS)

OBJECTS = $(C_OBJECTS) $(ASM_OBJECTS)

CONFIG_HEADERS = $(CONFIG_DIRECTORY)/device_manager_cnfg.h \
                 $(CONFIG_DIRECTORY)/nrf_drv_config.h \
                 $(CONFIG_DIRECTORY)/pstorage_platform.h \
                 $(CONFIG_DIRECTORY)/twi_master_config.h

# Initialize list of the header dependency files, one per object file.
DEPFILES := $(patsubst %.o,%.d,$(OBJECTS))


.PHONY: all sdk generate_patches help clean flash flash_softdevice

# default target - first one defined
all: $(OUTPUT_DIRECTORY)/$(PROJECT_NAME).bin \
     $(OUTPUT_DIRECTORY)/$(PROJECT_NAME).hex \
     $(OUTPUT_DIRECTORY)/$(PROJECT_NAME).disasm

# sdk target is used to download SDK from Nordic website.
sdk: $(SDK_DIRECTORY)

# Used to generate patches for configuration headers that can be committed to git.
# These patches are then automatically applied during the build process.
define generate_patch #,sdk_directory,header_filename
    diff -u $1/$2 config/$2 >config/$2.patch & exit 0
endef

generate_patches:
	@echo Generating config file patches.
	$(NO_ECHO) $(call generate_patch,$(SDK_GLS_EXAMPLE_DIRECTORY)/ble_app_gls_s110_pca10028,nrf_drv_config.h)
	$(NO_ECHO) $(call generate_patch,$(SDK_GLS_EXAMPLE_DIRECTORY),device_manager_cnfg.h)
	$(NO_ECHO) $(call generate_patch,$(SDK_GLS_EXAMPLE_DIRECTORY),pstorage_platform.h)
	$(NO_ECHO) $(call generate_patch,$(SDK_GLS_EXAMPLE_DIRECTORY),twi_master_config.h)

# target for printing all targets
help:
	@echo The following targets are available:
	@echo   sdk
	@echo   all
	@echo   flash_softdevice
	@echo   flash
	@echo   clean
	@echo   generate_patches

# Compile C source files.
$(OUTPUT_DIRECTORY)/%.o: %.c
	@echo Compiling $(notdir $<)
	$(NO_ECHO)$(CC) $(CFLAGS) $(INC_PATHS) -c -o $@ $<

# Assemble files
$(OUTPUT_DIRECTORY)/%.o: %.s
	@echo Assembling $(notdir $<)
	$(NO_ECHO)$(CC) $(ASMFLAGS) $(INC_PATHS) -c -o $@ $<

# Create build output directory
$(OUTPUT_DIRECTORY):
	@echo Creating $(OUTPUT_DIRECTORY)
	$(NO_ECHO)$(MK) $@

# Create configuration header directories
$(CONFIG_DIRECTORY):
	@echo Creating $(CONFIG_DIRECTORY)
	$(NO_ECHO)$(MK) $@

# Create configuration headers for this program based on ones from the SDK examples.
$(filter-out %nrf_drv_config.h,$(CONFIG_HEADERS)) : $(CONFIG_DIRECTORY)/%.h: $(SDK_GLS_EXAMPLE_DIRECTORY)/%.h $(CONFIG_DIRECTORY)/%.h.patch
	@echo Creating $@ from $^
	$(NO_ECHO)$(CP) $< $@
	$(NO_ECHO)patch -p0 -N -i $(CONFIG_DIRECTORY)/$*.h.patch

$(filter %nrf_drv_config.h,$(CONFIG_HEADERS)) : $(CONFIG_DIRECTORY)/%.h: $(SDK_GLS_EXAMPLE_DIRECTORY)/ble_app_gls_s110_pca10028/%.h $(CONFIG_DIRECTORY)/%.h.patch
	@echo Creating $@ from $^
	$(NO_ECHO)$(CP) $< $@
	$(NO_ECHO)patch -p0 -N -i $(CONFIG_DIRECTORY)/$*.h.patch

# The SDK source code only exists after it has been unzipped from the SDK archive.
$(SDK_DIRECTORY): ../$(SDK_ZIP)
	@echo Unzipping $<
	$(NO_ECHO)unzip -d $@ $<

# Download the SDK archive from Nordic website.
../$(SDK_ZIP):
	@echo Downloading $@
	$(NO_ECHO)curl -L0 $(SDK_URL) >$@

# Link
$(OUTPUT_DIRECTORY)/$(PROJECT_NAME).elf: $(OUTPUT_DIRECTORY) $(CONFIG_DIRECTORY) $(CONFIG_HEADERS) $(OBJECTS)
	@echo Linking $@
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@
	$(NO_ECHO)$(SIZE) $@
	-@echo ''

# Create binary .bin file from the .elf file
$(OUTPUT_DIRECTORY)/$(PROJECT_NAME).bin: $(OUTPUT_DIRECTORY)/$(PROJECT_NAME).elf
	@echo Generating $@
	$(NO_ECHO)$(OBJCOPY) -O binary $< $@

# Create binary .hex file from the .elf file
$(OUTPUT_DIRECTORY)/$(PROJECT_NAME).hex: $(OUTPUT_DIRECTORY)/$(PROJECT_NAME).elf
	@echo Generating $@
	$(NO_ECHO)$(OBJCOPY) -O ihex $< $@

# Create disassembly file from the .elf file
$(OUTPUT_DIRECTORY)/$(PROJECT_NAME).disasm: $(OUTPUT_DIRECTORY)/$(PROJECT_NAME).elf
	@echo Generating $@
	$(NO_ECHO)$(OBJDUMP) -d -f -M reg-names-std --demangle $< >$@

clean:
	@echo Cleaning out $(OUTPUT_DIRECTORY)
	$(NO_ECHO)$(RM) $(OUTPUT_DIRECTORY)

flash: $(OUTPUT_DIRECTORY)/$(PROJECT_NAME).hex
	@echo Flashing  $(notdir $<)
	$(NRF51_TOOL_PATH)/nrfjprog/nrfjprog --program $< -f nrf51  --sectorerase
	$(NRF51_TOOL_PATH)/nrfjprog/nrfjprog --reset

flash_softdevice:
	@echo Flashing s110_nrf51_8.0.0_softdevice.hex
	$(NRF51_TOOL_PATH)/nrfjprog/nrfjprog --program $(NRF51_SDK_PATH)/components/softdevice/s110/hex/s110_nrf51_8.0.0_softdevice.hex -f nrf51 --chiperase
	$(NRF51_TOOL_PATH)/nrfjprog/nrfjprog --reset


# Pull in all library header dependency rules
-include $(DEPFILES)
