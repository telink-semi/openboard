################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/Mars_B91_Mother_V2.2/device/button.c \
E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/Mars_B91_Mother_V2.2/device/lis2dh.c \
E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/Mars_B91_Mother_V2.2/device/oled.c \
E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/Mars_B91_Mother_V2.2/device/photoresistor.c \
E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/Mars_B91_Mother_V2.2/device/serial_port.c \
E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/Mars_B91_Mother_V2.2/device/sht30.c \
E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/Mars_B91_Mother_V2.2/device/ws2812b.c 

OBJS += \
./vendor/Mars_B91_Mother_V2.2/device/button.o \
./vendor/Mars_B91_Mother_V2.2/device/lis2dh.o \
./vendor/Mars_B91_Mother_V2.2/device/oled.o \
./vendor/Mars_B91_Mother_V2.2/device/photoresistor.o \
./vendor/Mars_B91_Mother_V2.2/device/serial_port.o \
./vendor/Mars_B91_Mother_V2.2/device/sht30.o \
./vendor/Mars_B91_Mother_V2.2/device/ws2812b.o 

C_DEPS += \
./vendor/Mars_B91_Mother_V2.2/device/button.d \
./vendor/Mars_B91_Mother_V2.2/device/lis2dh.d \
./vendor/Mars_B91_Mother_V2.2/device/oled.d \
./vendor/Mars_B91_Mother_V2.2/device/photoresistor.d \
./vendor/Mars_B91_Mother_V2.2/device/serial_port.d \
./vendor/Mars_B91_Mother_V2.2/device/sht30.d \
./vendor/Mars_B91_Mother_V2.2/device/ws2812b.d 


# Each subdirectory must supply rules for building sources it contributes
vendor/Mars_B91_Mother_V2.2/device/button.o: E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/Mars_B91_Mother_V2.2/device/button.c vendor/Mars_B91_Mother_V2.2/device/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\chip\B91\drivers" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\common" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

vendor/Mars_B91_Mother_V2.2/device/lis2dh.o: E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/Mars_B91_Mother_V2.2/device/lis2dh.c vendor/Mars_B91_Mother_V2.2/device/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\chip\B91\drivers" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\common" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

vendor/Mars_B91_Mother_V2.2/device/oled.o: E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/Mars_B91_Mother_V2.2/device/oled.c vendor/Mars_B91_Mother_V2.2/device/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\chip\B91\drivers" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\common" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

vendor/Mars_B91_Mother_V2.2/device/photoresistor.o: E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/Mars_B91_Mother_V2.2/device/photoresistor.c vendor/Mars_B91_Mother_V2.2/device/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\chip\B91\drivers" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\common" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

vendor/Mars_B91_Mother_V2.2/device/serial_port.o: E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/Mars_B91_Mother_V2.2/device/serial_port.c vendor/Mars_B91_Mother_V2.2/device/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\chip\B91\drivers" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\common" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

vendor/Mars_B91_Mother_V2.2/device/sht30.o: E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/Mars_B91_Mother_V2.2/device/sht30.c vendor/Mars_B91_Mother_V2.2/device/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\chip\B91\drivers" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\common" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

vendor/Mars_B91_Mother_V2.2/device/ws2812b.o: E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/Mars_B91_Mother_V2.2/device/ws2812b.c vendor/Mars_B91_Mother_V2.2/device/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\chip\B91\drivers" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\common" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


