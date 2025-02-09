################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/LPC845_CPP_R2052.cpp \
../src/cr_cpp_config.cpp \
../src/cr_startup_lpc84x.cpp 

C_SRCS += \
../src/crp.c \
../src/mtb.c 

S_SRCS += \
../src/aeabi_romdiv_patch.s 

CPP_DEPS += \
./src/LPC845_CPP_R2052.d \
./src/cr_cpp_config.d \
./src/cr_startup_lpc84x.d 

C_DEPS += \
./src/crp.d \
./src/mtb.d 

OBJS += \
./src/LPC845_CPP_R2052.o \
./src/aeabi_romdiv_patch.o \
./src/cr_cpp_config.o \
./src/cr_startup_lpc84x.o \
./src/crp.o \
./src/mtb.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M0PLUS -D__MTB_BUFFER_SIZE=256 -D__USE_ROMDIVIDE -DCPP_USE_HEAP -D__LPC84X__ -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\inc" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\APLICACION" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\SENSOR_Driver" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\ADC_Driver" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\MDE" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\GPIOPWM_Driver" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\MAQUINASDEESTADOS" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\RC522" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\SPI" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\UART0_Driver" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\UTILS\CALLBACK" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\TIMERSW_Driver" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\SYSTICK_Driver" -I"C:\Users\matia\Documents\MCUXpressoIDE_11.10.0_3148\workspace\LPC845_CPP_R2053_LCD\src\GPIO_Driver" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.s src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU Assembler'
	arm-none-eabi-gcc -c -x assembler-with-cpp -D__NEWLIB__ -DDEBUG -D__CODE_RED -DCORE_M0PLUS -D__USE_ROMDIVIDE -D__LPC84X__ -g3 -gdwarf-4 -mcpu=cortex-m0 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M0PLUS -D__MTB_BUFFER_SIZE=256 -D__USE_ROMDIVIDE -DCPP_USE_HEAP -D__LPC84X__ -Os -fno-common -g3 -gdwarf-4 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/LPC845_CPP_R2052.d ./src/LPC845_CPP_R2052.o ./src/aeabi_romdiv_patch.o ./src/cr_cpp_config.d ./src/cr_cpp_config.o ./src/cr_startup_lpc84x.d ./src/cr_startup_lpc84x.o ./src/crp.d ./src/crp.o ./src/mtb.d ./src/mtb.o

.PHONY: clean-src

