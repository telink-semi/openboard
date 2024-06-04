################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
E:/promotion/gitlab/opensdk/Mars_B91/telink_rocket_driver_sdk/chip/B91/boot/cstartup_flash.S \
E:/promotion/gitlab/opensdk/Mars_B91/telink_rocket_driver_sdk/chip/B91/boot/cstartup_ram.S 

OBJS += \
./boot/cstartup_flash.o \
./boot/cstartup_ram.o 

S_UPPER_DEPS += \
./boot/cstartup_flash.d \
./boot/cstartup_ram.d 


# Each subdirectory must supply rules for building sources it contributes
boot/cstartup_flash.o: E:/promotion/gitlab/opensdk/Mars_B91/telink_rocket_driver_sdk/chip/B91/boot/cstartup_flash.S boot/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross Assembler'
	riscv32-elf-gcc -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -x assembler-with-cpp -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_rocket_driver_sdk\chip\B91\drivers" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_rocket_driver_sdk\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_rocket_driver_sdk\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_rocket_driver_sdk\common" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_rocket_driver_sdk\demo\vendor\common\common" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

boot/cstartup_ram.o: E:/promotion/gitlab/opensdk/Mars_B91/telink_rocket_driver_sdk/chip/B91/boot/cstartup_ram.S boot/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross Assembler'
	riscv32-elf-gcc -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -x assembler-with-cpp -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_rocket_driver_sdk\chip\B91\drivers" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_rocket_driver_sdk\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_rocket_driver_sdk\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_rocket_driver_sdk\common" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_rocket_driver_sdk\demo\vendor\common\common" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


