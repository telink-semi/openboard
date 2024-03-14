################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/adc.c \
E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/aes.c \
E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/analog.c \
E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/audio.c \
E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/clock.c \
E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/emi.c \
E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/flash.c \
E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/gpio.c \
E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/i2c.c \
E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/lpc.c \
E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/mdec.c \
E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/pwm.c \
E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/s7816.c \
E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/spi.c \
E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/stimer.c \
E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/timer.c \
E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/uart.c \
E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/usbhw.c 

OBJS += \
./drivers/adc.o \
./drivers/aes.o \
./drivers/analog.o \
./drivers/audio.o \
./drivers/clock.o \
./drivers/emi.o \
./drivers/flash.o \
./drivers/gpio.o \
./drivers/i2c.o \
./drivers/lpc.o \
./drivers/mdec.o \
./drivers/pwm.o \
./drivers/s7816.o \
./drivers/spi.o \
./drivers/stimer.o \
./drivers/timer.o \
./drivers/uart.o \
./drivers/usbhw.o 

C_DEPS += \
./drivers/adc.d \
./drivers/aes.d \
./drivers/analog.d \
./drivers/audio.d \
./drivers/clock.d \
./drivers/emi.d \
./drivers/flash.d \
./drivers/gpio.d \
./drivers/i2c.d \
./drivers/lpc.d \
./drivers/mdec.d \
./drivers/pwm.d \
./drivers/s7816.d \
./drivers/spi.d \
./drivers/stimer.d \
./drivers/timer.d \
./drivers/uart.d \
./drivers/usbhw.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/adc.o: E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/adc.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\chip\B91\drivers" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\common" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/aes.o: E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/aes.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\chip\B91\drivers" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\common" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/analog.o: E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/analog.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\chip\B91\drivers" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\common" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/audio.o: E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/audio.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\chip\B91\drivers" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\common" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/clock.o: E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/clock.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\chip\B91\drivers" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\common" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/emi.o: E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/emi.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\chip\B91\drivers" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\common" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/flash.o: E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/flash.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\chip\B91\drivers" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\common" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/gpio.o: E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/gpio.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\chip\B91\drivers" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\common" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/i2c.o: E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/i2c.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\chip\B91\drivers" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\common" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/lpc.o: E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/lpc.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\chip\B91\drivers" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\common" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/mdec.o: E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/mdec.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\chip\B91\drivers" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\common" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/pwm.o: E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/pwm.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\chip\B91\drivers" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\common" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/s7816.o: E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/s7816.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\chip\B91\drivers" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\common" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/spi.o: E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/spi.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\chip\B91\drivers" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\common" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/stimer.o: E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/stimer.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\chip\B91\drivers" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\common" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/timer.o: E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/timer.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\chip\B91\drivers" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\common" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/uart.o: E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/uart.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\chip\B91\drivers" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\common" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/usbhw.o: E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/chip/B91/drivers/usbhw.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\chip\B91\drivers" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\common" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


