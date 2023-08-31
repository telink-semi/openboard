################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include vendor/common/flash/subdir.mk
-include vendor/common/calibration/subdir.mk
-include vendor/common/subdir.mk
-include vendor/Mars_B91_Mother_V2.2/example/subdir.mk
-include vendor/Mars_B91_Mother_V2.2/device/subdir.mk
-include vendor/Mars_B91_Mother_V2.2/subdir.mk
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


BUILD_ARTIFACT_NAME := Mars_B91_Mother_V2.2
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
Mars_B91_Mother_V2.2.bin \

SECONDARY_LIST += \
Mars_B91_Mother_V2.2.lst \

SECONDARY_SIZE += \
Mars_B91_Mother_V2.2.siz \


# All Target
all: main-build

# Main-build Target
main-build: Mars_B91_Mother_V2.2.elf secondary-outputs

# Tool invocations
Mars_B91_Mother_V2.2.elf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU RISC-V Cross C Linker'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -T "E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\chip\B91\link\flash_boot.link" -nostartfiles -Xlinker --gc-sections -L"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\chip\B91\drivers\lib" -Wl,-Map,"Mars_B91_Mother_V2.2.map" -fpack-struct -fshort-enums -g3 -mcpu=d25f -ffunction-sections -fdata-sections -mext-dsp -mabi=ilp32f -fmessage-length=0 -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -Wno-gnu-zero-variadic-macro-arguments  -O2 -o "Mars_B91_Mother_V2.2.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Mars_B91_Mother_V2.2.bin: Mars_B91_Mother_V2.2.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GNU RISC-V Cross Create Flash Image'
	riscv32-elf-objcopy -O binary "Mars_B91_Mother_V2.2.elf"  "Mars_B91_Mother_V2.2.bin"
	@echo 'Finished building: $@'
	@echo ' '

Mars_B91_Mother_V2.2.lst: Mars_B91_Mother_V2.2.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GNU RISC-V Cross Create Listing'
	riscv32-elf-objdump --source --all-headers --demangle --line-numbers --wide "Mars_B91_Mother_V2.2.elf" > "Mars_B91_Mother_V2.2.lst"
	@echo 'Finished building: $@'
	@echo ' '

Mars_B91_Mother_V2.2.siz: Mars_B91_Mother_V2.2.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GNU RISC-V Cross Print Size'
	riscv32-elf-size "Mars_B91_Mother_V2.2.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) Mars_B91_Mother_V2.2.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents main-build

-include ../makefile.targets
