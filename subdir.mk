################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../main.c 

OBJS += \
./main.o 

C_DEPS += \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: SDSCC Compiler'
	sdscc -DPLATFORM_ZCU102 -DHLS_NO_XIL_FPO_LIB -DWITH_SDSOC -DSAMPLE_OPTICAL_FLOW -DOF_PIX_PER_CLOCK=2 -Wall -O0 -g -I"../src" -I"../src/../top" -I/home/T0194096/Desktop/zcu102_rv_ss/sw/a53_linux/inc/video_lib -I/home/T0194096/Desktop/zcu102_rv_ss/sw/a53_linux/inc/xfopencv -c -fmessage-length=0 -MT"$@" -I/home/T0194096/Desktop/zcu102_rv_ss/sw/sysroot/usr/include/c++/6.2.1 -I/home/T0194096/Desktop/zcu102_rv_ss/sw/sysroot/usr/include/c++/6.2.1/aarch64-xilinx-linux -I/home/T0194096/Desktop/zcu102_rv_ss/sw/sysroot/usr/include/c++/6.2.1/backward -I/home/T0194096/Desktop/zcu102_rv_ss/sw/sysroot/usr/include -I/home/T0194096/Desktop/zcu102_rv_ss/sw/sysroot/usr/include/glib-2.0 -I/home/T0194096/Desktop/zcu102_rv_ss/sw/sysroot/usr/lib/glib-2.0/include -I/APPS/Xilinx/2017.4/SDK/2017.4/gnu/aarch64/nt/aarch64-linux/aarch64-linux-gnu/include/c++/6.2.1 -I/APPS/Xilinx/2017.4/SDK/2017.4/gnu/aarch64/nt/aarch64-linux/aarch64-linux-gnu/include/c++/6.2.1/aarch64-linux-gnu -I/APPS/Xilinx/2017.4/SDK/2017.4/gnu/aarch64/nt/aarch64-linux/aarch64-linux-gnu/include/c++/6.2.1/backward -I/APPS/Xilinx/2017.4/SDK/2017.4/gnu/aarch64/nt/aarch64-linux/lib/gcc/aarch64-linux-gnu/6.2.1/include -I/APPS/Xilinx/2017.4/SDK/2017.4/gnu/aarch64/nt/aarch64-linux/lib/gcc/aarch64-linux-gnu/6.2.1/include-fixed -I/APPS/Xilinx/2017.4/SDK/2017.4/gnu/aarch64/nt/aarch64-linux/aarch64-linux-gnu/include -Wno-overloaded-virtual -Wno-unused-label -Wno-strict-overflow -Wno-uninitialized -Wno-unused-function -Wno-unused-variable -Wno-unknown-attributes -Wno-unused-local-typedefs -Wno-sign-compare -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<" -sds-sys-config a53_linux -sds-proc a53_0 -sds-pf "/home/T0194096/Desktop/zcu102_rv_ss"
	@echo 'Finished building: $<'
	@echo ' '


