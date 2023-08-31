################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/UART_DEMO/app.c \
E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/UART_DEMO/app_dma.c \
E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/UART_DEMO/app_dma_llp.c \
E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/UART_DEMO/main.c 

OBJS += \
./vendor/UART_DEMO/app.o \
./vendor/UART_DEMO/app_dma.o \
./vendor/UART_DEMO/app_dma_llp.o \
./vendor/UART_DEMO/main.o 

C_DEPS += \
./vendor/UART_DEMO/app.d \
./vendor/UART_DEMO/app_dma.d \
./vendor/UART_DEMO/app_dma_llp.d \
./vendor/UART_DEMO/main.d 


# Each subdirectory must supply rules for building sources it contributes
vendor/UART_DEMO/app.o: E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/UART_DEMO/app.c vendor/UART_DEMO/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\chip\B91\drivers" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\common" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

vendor/UART_DEMO/app_dma.o: E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/UART_DEMO/app_dma.c vendor/UART_DEMO/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\chip\B91\drivers" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\common" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

vendor/UART_DEMO/app_dma_llp.o: E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/UART_DEMO/app_dma_llp.c vendor/UART_DEMO/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\chip\B91\drivers" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\common" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

vendor/UART_DEMO/main.o: E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/UART_DEMO/main.c vendor/UART_DEMO/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\chip\B91\drivers" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\common" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


