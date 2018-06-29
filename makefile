################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf _sds sd_card

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: pre-build main-build

# Main-build Target
main-build: of2_edit.elf

# Tool invocations
of2_edit.elf: $(OBJS)  $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: SDS++ Linker'
	sds++ -L/home/T0194096/Desktop/zcu102_rv_ss/sw/a53_linux/lib --sysroot=/home/T0194096/Desktop/zcu102_rv_ss/sw/sysroot -L=/lib -L=/usr/lib -Wl,-rpath-link=/home/T0194096/Desktop/zcu102_rv_ss/sw/sysroot/lib,-rpath-link=/home/T0194096/Desktop/zcu102_rv_ss/sw/sysroot/usr/lib --remote_ip_cache /home/T0194096/Desktop/live/ip_cache -o "of2_edit.elf" $(OBJS) $(USER_OBJS) $(LIBS) -dmclkid 2 -mno-bitstream  -mno-boot-files  -sds-sys-config a53_linux -sds-proc a53_0 -sds-pf "/home/T0194096/Desktop/zcu102_rv_ss"
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(OBJS)$(CXX_DEPS)$(C_DEPS)$(CPP_DEPS) of2_edit.elf
	-@echo ' '

pre-build:
	-sdsoc_make_clean Debug
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: main-build pre-build

-include ../makefile.targets
