################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/Mars_B91_Mother_V2.2/app_all.c \
E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/Mars_B91_Mother_V2.2/main.c 

OBJS += \
./vendor/Mars_B91_Mother_V2.2/app_all.o \
./vendor/Mars_B91_Mother_V2.2/main.o 

C_DEPS += \
./vendor/Mars_B91_Mother_V2.2/app_all.d \
./vendor/Mars_B91_Mother_V2.2/main.d 


# Each subdirectory must supply rules for building sources it contributes
vendor/Mars_B91_Mother_V2.2/app_all.o: E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/Mars_B91_Mother_V2.2/app_all.c vendor/Mars_B91_Mother_V2.2/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\chip\B91\drivers" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\common" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

vendor/Mars_B91_Mother_V2.2/main.o: E:/promotion/gitlab/telink_b91m_driver_sdk/telink_b91m_driver_sdk/demo/vendor/Mars_B91_Mother_V2.2/main.c vendor/Mars_B91_Mother_V2.2/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\chip\B91\drivers" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\common" -I"E:\promotion\gitlab\telink_b91m_driver_sdk\telink_b91m_driver_sdk\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


