################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/demo/vendor/common/B91/flash/puya_common.c \
E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/demo/vendor/common/B91/flash/puya_flash_scratch.c \
E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/demo/vendor/common/B91/flash/puya_flash_trim.c 

OBJS += \
./vendor/common/flash/puya_common.o \
./vendor/common/flash/puya_flash_scratch.o \
./vendor/common/flash/puya_flash_trim.o 

C_DEPS += \
./vendor/common/flash/puya_common.d \
./vendor/common/flash/puya_flash_scratch.d \
./vendor/common/flash/puya_flash_trim.d 


# Each subdirectory must supply rules for building sources it contributes
vendor/common/flash/puya_common.o: E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/demo/vendor/common/B91/flash/puya_common.c vendor/common/flash/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\chip\B91\drivers" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\common" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

vendor/common/flash/puya_flash_scratch.o: E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/demo/vendor/common/B91/flash/puya_flash_scratch.c vendor/common/flash/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\chip\B91\drivers" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\common" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

vendor/common/flash/puya_flash_trim.o: E:/promotion/gitlab/opensdk/Mars_B91/telink_mars_b91_driver_sdk_v2.2.0/demo/vendor/common/B91/flash/puya_flash_trim.c vendor/common/flash/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -Werror -Wall -Wextra -Wshadow  -g3 -DMCU_STARTUP_FLASH=1 -DMCU_CORE_B91=1 -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\chip\B91\drivers" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\flash" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\B91\calibration" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\common" -I"E:\promotion\gitlab\opensdk\Mars_B91\telink_mars_b91_driver_sdk_v2.2.0\demo\vendor\common\common" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\lib\gcc\riscv32-elf\7.4.0\include" -I"C:\TelinkIoTStudio\\RDS\V3.2.3\toolchains\nds32le-elf-mculib-v5f\riscv32-elf\include" -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -std=c99 -fpack-struct -fshort-enums -fno-jump-tables -mcmodel=medium -Wno-nonnull-compare -Wall -Wextra -Wshadow -Werror -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


