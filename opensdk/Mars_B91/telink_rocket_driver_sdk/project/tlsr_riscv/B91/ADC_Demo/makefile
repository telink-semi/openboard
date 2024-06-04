################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include vendor/common/flash/subdir.mk
-include vendor/common/calibration/subdir.mk
-include vendor/common/auto_test/subdir.mk
-include vendor/common/subdir.mk
-include vendor/ADC_Demo/subdir.mk
-include drivers/flash/subdir.mk
-include drivers/subdir.mk
-include common/usb_dbg/subdir.mk
-include common/bt_debug/subdir.mk
-include common/subdir.mk
-include boot/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := ADC_Demo
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
ADC_Demo.bin \

SECONDARY_LIST += \
ADC_Demo.lst \

SECONDARY_SIZE += \
ADC_Demo.siz \


# All Target
all: main-build

# Main-build Target
main-build: ADC_Demo.elf secondary-outputs

# Tool invocations
ADC_Demo.elf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU RISC-V Cross C Linker'
	riscv32-elf-gcc -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -T "E:\promotion\gitlab\opensdk\Mars_B91\telink_rocket_driver_sdk\chip\B91\link\flash_boot.link" -nostartfiles -Xlinker --gc-sections -L"E:\promotion\gitlab\opensdk\Mars_B91\telink_rocket_driver_sdk\chip\B91\drivers\lib" -Wl,-Map,"ADC_Demo.map" -fpack-struct -fshort-enums -g3 -mcpu=d25f -ffunction-sections -fdata-sections -mext-dsp -mabi=ilp32f -fmessage-length=0 -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -Wno-gnu-zero-variadic-macro-arguments  -O2 -o "ADC_Demo.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

ADC_Demo.bin: ADC_Demo.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GNU RISC-V Cross Create Flash Image'
	riscv32-elf-objcopy -O binary "ADC_Demo.elf"  "ADC_Demo.bin"
	@echo 'Finished building: $@'
	@echo ' '

ADC_Demo.lst: ADC_Demo.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GNU RISC-V Cross Create Listing'
	riscv32-elf-objdump --source --all-headers --demangle --line-numbers --wide "ADC_Demo.elf" > "ADC_Demo.lst"
	@echo 'Finished building: $@'
	@echo ' '

ADC_Demo.siz: ADC_Demo.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GNU RISC-V Cross Print Size'
	riscv32-elf-size "ADC_Demo.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) ADC_Demo.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents main-build

-include ../makefile.targets
