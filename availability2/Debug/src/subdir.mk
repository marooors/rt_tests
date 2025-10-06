################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/availability.c \
../src/getCpuFrequency.c \
../src/setupRtThread.c 

C_DEPS += \
./src/availability.d \
./src/getCpuFrequency.d \
./src/setupRtThread.d 

OBJS += \
./src/availability.o \
./src/getCpuFrequency.o \
./src/setupRtThread.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/usr/include -O0 -g3 -Wall -c -fmessage-length=0 -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/availability.d ./src/availability.o ./src/getCpuFrequency.d ./src/getCpuFrequency.o ./src/setupRtThread.d ./src/setupRtThread.o

.PHONY: clean-src

