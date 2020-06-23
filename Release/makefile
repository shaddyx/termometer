################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include libraries/LiquidCrystal/src/subdir.mk
-include core/core/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
AR += \
/home/shaddy/sloeber-workspace/termometer/Release/arduino.ar \

ELF += \
termometer.elf \

HEX += \
termometer.hex \


# All Target
all: termometer

# Tool invocations
termometer: $(HEX)
	@echo 'Building target: $@'
	@echo 'Printing size:'
	"/home/shaddy/devel/Sloeber//arduinoPlugin/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino5/bin/avr-size" -A "/home/shaddy/sloeber-workspace/termometer/Release/termometer.elf"
	@echo 'Finished building target: $@'
	@echo ' '

/home/shaddy/sloeber-workspace/termometer/Release/arduino.ar: $(AR_OBJ)
	@echo 'Starting archiver'
	"/home/shaddy/devel/Sloeber//arduinoPlugin/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino5/bin/avr-gcc-ar" rcs     /home/shaddy/sloeber-workspace/termometer/Release/arduino.ar $(AR_OBJ)
	@echo 'Finished building: $@'
	@echo ' '

termometer.elf: $(LINK_OBJ) $(AR)
	@echo 'Starting combiner'
	"/home/shaddy/devel/Sloeber//arduinoPlugin/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino5/bin/avr-gcc" -Wall -Wextra -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega328p  -o "/home/shaddy/sloeber-workspace/termometer/Release/termometer.elf"   $(LINK_OBJ)   /home/shaddy/sloeber-workspace/termometer/Release/arduino.ar  "-L/home/shaddy/sloeber-workspace/termometer/Release" -lm
	@echo 'Finished building: $@'
	@echo ' '

termometer.hex: $(ELF)
	@echo 'Do all objcopy commands'
	${A.RECIPE.OBJCOPY.EEP.PATTERN}
	${A.RECIPE.OBJCOPY.HEX.PATTERN}
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C_UPPER_DEPS)$(PDE_DEPS)$(C_DEPS)$(AR)$(CC_DEPS)$(AR_OBJ)$(C++_DEPS)$(LINK_OBJ)$(CXX_DEPS)$(ASM_DEPS)$(HEX)$(INO_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(ELF)$(CPP_DEPS)
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
