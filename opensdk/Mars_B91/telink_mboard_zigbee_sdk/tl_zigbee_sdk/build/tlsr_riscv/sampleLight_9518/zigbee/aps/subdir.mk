################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
E:/promotion/gitlab/opensdk/Mars_B91/telink_mboard_zigbee_sdk/tl_zigbee_sdk/zigbee/aps/aps_group.c 

OBJS += \
./zigbee/aps/aps_group.o 

C_DEPS += \
./zigbee/aps/aps_group.d 


# Each subdirectory must supply rules for building sources it contributes
zigbee/aps/aps_group.o: /cygdrive/E/promotion/gitlab/opensdk/Mars_B91/telink_mboard_zigbee_sdk/tl_zigbee_sdk/zigbee/aps/aps_group.c
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -DMCU_CORE_B91=1 -DROUTER=1 -D__PROJECT_TL_DIMMABLE_LIGHT__=1 -I../../../apps/common -I../../../apps/sampleLight -I../../../platform -I../../../proj/common -I../../../proj -I../../../zigbee/common/includes -I../../../zigbee/zbapi -I../../../zigbee/bdb/includes -I../../../zigbee/gp -I../../../zigbee/zcl -I../../../zigbee/ota -I../../../zbhci -O2 -mcmodel=small -fpack-struct -fshort-enums -flto -Wall -mcpu=d25f -ffunction-sections -fdata-sections -mext-dsp -mabi=ilp32f  -c -fmessage-length=0  -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -Wno-nonnull-compare -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


