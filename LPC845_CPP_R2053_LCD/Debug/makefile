################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/UTILS/CALLBACK/subdir.mk
-include src/UART0_Driver/subdir.mk
-include src/TIMERSW_Driver/subdir.mk
-include src/SYSTICK_Driver/subdir.mk
-include src/SPI/subdir.mk
-include src/SENSOR_Driver/subdir.mk
-include src/RC522/subdir.mk
-include src/MDE/subdir.mk
-include src/MAQUINASDEESTADOS/subdir.mk
-include src/GPIO_Driver/subdir.mk
-include src/GPIOPWM_Driver/subdir.mk
-include src/APLICACION/subdir.mk
-include src/ADC_Driver/subdir.mk
-include src/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++M_DEPS)),)
-include $(C++M_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CCM_DEPS)),)
-include $(CCM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXXM_DEPS)),)
-include $(CXXM_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := LPC845_CPP_R2053_LCD
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: LPC845_CPP_R2053_LCD.axf

# Tool invocations
LPC845_CPP_R2053_LCD.axf: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU C++ Linker'
	arm-none-eabi-c++ -nostdlib -Xlinker -Map="LPC845_CPP_R2053_LCD.map" -Xlinker --cref -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m0 -mthumb -T LPC845_CPP_R2053_LCD_Debug.ld -o "LPC845_CPP_R2053_LCD.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) LPC845_CPP_R2053_LCD.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "LPC845_CPP_R2053_LCD.axf"; # arm-none-eabi-objcopy -v -O binary "LPC845_CPP_R2053_LCD.axf" "LPC845_CPP_R2053_LCD.bin" ; # checksum -p LPC845 -d "LPC845_CPP_R2053_LCD.bin";
	-@echo ' '

.PHONY: all clean dependents main-build post-build

-include ../makefile.targets
