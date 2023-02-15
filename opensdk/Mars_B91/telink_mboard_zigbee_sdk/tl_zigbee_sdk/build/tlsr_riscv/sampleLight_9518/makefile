################################################################################
# Automatically-generated file. Do not edit!
################################################################################

ifndef ANDESIGHT_ROOT
ANDESIGHT_ROOT=/cygdrive/C/Andestech/AndeSight_RDS_v323
endif

ifndef CROSS_COMPILE
CROSS_COMPILE=riscv32-elf-
endif

ifndef SECONDARY_OUTPUT_PATH
SECONDARY_OUTPUT_PATH=output
endif

$(shell mkdir -p $(SECONDARY_OUTPUT_PATH))

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include apps/common/subdir.mk
-include apps/sampleLight/subdir.mk
-include platform/boot/b91/subdir.mk
-include platform/boot/subdir.mk
-include platform/chip_b91/subdir.mk
-include platform/services/subdir.mk
-include proj/common/subdir.mk
-include proj/drivers/subdir.mk
-include proj/drivers/usb/app/subdir.mk
-include proj/drivers/usb/subdir.mk
-include proj/os/subdir.mk
-include zbhci/uart/subdir.mk
-include zbhci/usbCDC/subdir.mk
-include zbhci/usbHID/subdir.mk
-include zbhci/usbPrint/subdir.mk
-include zbhci/subdir.mk
-include zigbee/af/subdir.mk
-include zigbee/aps/subdir.mk
-include zigbee/bdb/subdir.mk
-include zigbee/common/subdir.mk
-include zigbee/gp/subdir.mk
-include zigbee/mac/subdir.mk
-include zigbee/ota/subdir.mk
-include zigbee/ss/subdir.mk
-include zigbee/wwah/subdir.mk
-include zigbee/zcl/closures/subdir.mk
-include zigbee/zcl/commissioning/subdir.mk
-include zigbee/zcl/general/subdir.mk
-include zigbee/zcl/hvac/subdir.mk
-include zigbee/zcl/light_color_control/subdir.mk
-include zigbee/zcl/measument_sensing/subdir.mk
-include zigbee/zcl/ota_upgrading/subdir.mk
-include zigbee/zcl/security_safety/subdir.mk
-include zigbee/zcl/smart_energy/subdir.mk
-include zigbee/zcl/subdir.mk
-include zigbee/zcl/zcl_wwah/subdir.mk
-include zigbee/zcl/zll_commissioning/subdir.mk
-include zigbee/zdo/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
BUILD_ARTIFACT_FILE_BASE_NAME = sampleLight_9518

LINKER_OUTPUTS = sampleLight_9518.elf

GPROF_OUT += \
../gmon.sum ../gmon.out \

PWR_OUT += \
$(SECONDARY_OUTPUT_PATH)/pwr \

GCOV_OUT += \
$(foreach subdir,$(SUBDIRS),$(wildcard $(subdir)/*.gcda) $(wildcard $(subdir)/*.gcno)) \

SYMBOL_OUTPUTS += \
$(SECONDARY_OUTPUT_PATH)/symbol.txt \

READELF_OUTPUTS += \
$(SECONDARY_OUTPUT_PATH)/readelf.txt \

OBJDUMP_OUTPUTS += \
$(SECONDARY_OUTPUT_PATH)/objdump.txt \

SIZE_OUTPUTS += \
$(SECONDARY_OUTPUT_PATH)/.PHONY.size \


# All Target
all:
	$(MAKE) --no-print-directory pre-build
	$(MAKE) --no-print-directory main-build

# Main-build Target
main-build: sampleLight_9518.elf secondary-outputs

# Tool invocations
sampleLight_9518.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Andes C Linker'
	$(CROSS_COMPILE)gcc -O2 -nostartfiles -static -L../../../zigbee/lib/riscv -L../../../platform/lib -flto -mcmodel=small -fpack-struct -fshort-enums -mcpu=d25f -ffunction-sections -fdata-sections -mext-dsp -mabi=ilp32f -fmessage-length=0 -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -T"/cygdrive/E/promotion/gitlab/opensdk/Mars_B91/telink_mboard_zigbee_sdk/tl_zigbee_sdk/build/tlsr_riscv/boot.link" -Wl,--gc-sections -o "sampleLight_9518.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

$(foreach subdir,$(SUBDIRS),$(wildcard $(subdir)/*.gcda) $(wildcard $(subdir)/*.gcno)): sampleLight_9518.elf $(OBJS) $(USER_OBJS)
../gmon.sum ../gmon.out: sampleLight_9518.elf $(OBJS) $(USER_OBJS)
$(SECONDARY_OUTPUT_PATH)/pwr: sampleLight_9518.elf $(OBJS) $(USER_OBJS)

$(SECONDARY_OUTPUT_PATH)/symbol.txt: $(LINKER_OUTPUTS)
	@echo 'Invoking: NM (symbol listing)'
	$(CROSS_COMPILE)nm -n -l -C "sampleLight_9518.elf" > $(SECONDARY_OUTPUT_PATH)/symbol.txt
	@echo 'Finished building: $@'
	@echo ' '

$(SECONDARY_OUTPUT_PATH)/readelf.txt: $(LINKER_OUTPUTS)
	@echo 'Invoking: Readelf (ELF info listing)'
	$(CROSS_COMPILE)readelf -a "sampleLight_9518.elf" > $(SECONDARY_OUTPUT_PATH)/readelf.txt
	@echo 'Finished building: $@'
	@echo ' '

$(SECONDARY_OUTPUT_PATH)/objdump.txt: $(LINKER_OUTPUTS)
	@echo 'Invoking: Objdump (disassembly)'
	$(CROSS_COMPILE)objdump -x -d -C "sampleLight_9518.elf" > $(SECONDARY_OUTPUT_PATH)/objdump.txt
	@echo 'Finished building: $@'
	@echo ' '

: $(LINKER_OUTPUTS)
	@echo 'Invoking: Objcopy (object content copy)'
	$(CROSS_COMPILE)objcopy -S -O binary "sampleLight_9518.elf"
	@echo 'Finished building: $@'
	@echo ' '

$(SECONDARY_OUTPUT_PATH)/.PHONY.size: $(LINKER_OUTPUTS)
	@echo 'Invoking: Size (section size listing)'
	$(CROSS_COMPILE)size  "sampleLight_9518.elf" | tee $(SECONDARY_OUTPUT_PATH)/.PHONY.size
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GPROF_OUT)$(PWR_OUT)$(SIZE_OUTPUTS)$(READELF_OUTPUTS)$(EXECUTABLES)$(OBJDUMP_OUTPUTS)$(OBJS)$(SYMBOL_OUTPUTS)$(S_UPPER_DEPS)$(GCOV_OUT)$(C_DEPS) sampleLight_9518.elf
	-@echo ' '

pre-build:
	-"../../../tools/tl_link_load.sh" "../../../platform/boot/b91/boot_b91.link" "E:\promotion\gitlab\opensdk\Mars_B91\telink_mboard_zigbee_sdk\tl_zigbee_sdk\build\tlsr_riscv/boot.link"
	-@echo ' '

post-build:
	-"../../../tools/tl_check_fw.sh" sampleLight_9518 riscv
	-@echo ' '

secondary-outputs: $(SYMBOL_OUTPUTS) $(READELF_OUTPUTS) $(OBJDUMP_OUTPUTS) $(SIZE_OUTPUTS)

.PHONY: all clean dependents
.SECONDARY: main-build pre-build post-build

-include ../makefile.targets
