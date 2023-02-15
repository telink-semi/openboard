################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
E:/promotion/gitlab/opensdk/Mars_B91/telink_mboard_zigbee_sdk/tl_zigbee_sdk/zigbee/mac/mac_phy.c \
E:/promotion/gitlab/opensdk/Mars_B91/telink_mboard_zigbee_sdk/tl_zigbee_sdk/zigbee/mac/mac_pib.c 

OBJS += \
./zigbee/mac/mac_phy.o \
./zigbee/mac/mac_pib.o 

C_DEPS += \
./zigbee/mac/mac_phy.d \
./zigbee/mac/mac_pib.d 


# Each subdirectory must supply rules for building sources it contributes
zigbee/mac/mac_phy.o: /cygdrive/E/promotion/gitlab/opensdk/Mars_B91/telink_mboard_zigbee_sdk/tl_zigbee_sdk/zigbee/mac/mac_phy.c
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -DMCU_CORE_B91=1 -DCOORDINATOR=1 -D__PROJECT_TL_GW__=1 -I../../../apps/common -I../../../apps/sampleGW -I../../../platform -I../../../platform/riscv -I../../../proj/common -I../../../proj -I../../../zigbee/common/includes -I../../../zigbee/zbapi -I../../../zigbee/bdb/includes -I../../../zigbee/gp -I../../../zigbee/zcl -I../../../zigbee/ota -I../../../zbhci -O2 -mcmodel=small -fpack-struct -fshort-enums -flto -Wall -mcpu=d25f -ffunction-sections -fdata-sections -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -Wno-nonnull-compare -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

zigbee/mac/mac_pib.o: /cygdrive/E/promotion/gitlab/opensdk/Mars_B91/telink_mboard_zigbee_sdk/tl_zigbee_sdk/zigbee/mac/mac_pib.c
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -DMCU_CORE_B91=1 -DCOORDINATOR=1 -D__PROJECT_TL_GW__=1 -I../../../apps/common -I../../../apps/sampleGW -I../../../platform -I../../../platform/riscv -I../../../proj/common -I../../../proj -I../../../zigbee/common/includes -I../../../zigbee/zbapi -I../../../zigbee/bdb/includes -I../../../zigbee/gp -I../../../zigbee/zcl -I../../../zigbee/ota -I../../../zbhci -O2 -mcmodel=small -fpack-struct -fshort-enums -flto -Wall -mcpu=d25f -ffunction-sections -fdata-sections -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -Wno-nonnull-compare -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


