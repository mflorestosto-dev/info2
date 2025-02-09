################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/SYSTICK_Driver/SYSTICK.cpp 

CPP_DEPS += \
./src/SYSTICK_Driver/SYSTICK.d 

OBJS += \
./src/SYSTICK_Driver/SYSTICK.o 


# Each subdirectory must supply rules for building sources it contributes
src/SYSTICK_Driver/%.o: ../src/SYSTICK_Driver/%.cpp src/SYSTICK_Driver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M0PLUS -D__MTB_BUFFER_SIZE=256 -D__USE_ROMDIVIDE -DCPP_USE_HEAP -D__LPC84X__ -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\inc" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\APLICACION" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\SENSOR_Driver" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\ADC_Driver" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\MDE" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\GPIOPWM_Driver" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\MAQUINASDEESTADOS" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\RC522" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\SPI" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\UART0_Driver" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\UTILS\CALLBACK" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\TIMERSW_Driver" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\SYSTICK_Driver" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\GPIO_Driver" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-SYSTICK_Driver

clean-src-2f-SYSTICK_Driver:
	-$(RM) ./src/SYSTICK_Driver/SYSTICK.d ./src/SYSTICK_Driver/SYSTICK.o

.PHONY: clean-src-2f-SYSTICK_Driver

