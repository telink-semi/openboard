################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
E:/promotion/gitlab/opensdk/Mars_B91/telink_rocket_driver_sdk/demo/vendor/ADC_Demo/app.c \
E:/promotion/gitlab/opensdk/Mars_B91/telink_rocket_driver_sdk/demo/vendor/ADC_Demo/main.c 

OBJS += \
./vendor/ADC_Demo/app.o \
./vendor/ADC_Demo/main.o 

C_DEPS += \
./vendor/ADC_Demo/app.d \
./vendor/ADC_Demo/main.d 


# Each subdirectory must supply rules for building sources it contributes
vendor/ADC_Demo/app.o: E:/promotion/gitlab/opensdk/Mars_B91/telink_rocket_driver_sdk/demo/vendor/ADC_Demo/app.c vendor/ADC_Demo/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_rocket_driver_sdk\chip\B91\drivers" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_rocket_driver_sdk\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_rocket_driver_sdk\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_rocket_driver_sdk\common" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_rocket_driver_sdk\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

vendor/ADC_Demo/main.o: E:/promotion/gitlab/opensdk/Mars_B91/telink_rocket_driver_sdk/demo/vendor/ADC_Demo/main.c vendor/ADC_Demo/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_rocket_driver_sdk\chip\B91\drivers" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_rocket_driver_sdk\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_rocket_driver_sdk\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_rocket_driver_sdk\common" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_rocket_driver_sdk\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


