################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/main.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c 

CPP_SRCS += \
../Core/Src/TIM_IT.cpp \
../Core/Src/badminton.cpp \
../Core/Src/baseball_intake.cpp \
../Core/Src/mainpp.cpp \
../Core/Src/reset_button.cpp \
../Core/Src/ros_port.cpp \
../Core/Src/servo.cpp \
../Core/Src/shooter.cpp \
../Core/Src/shooter_base.cpp 

C_DEPS += \
./Core/Src/main.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d 

OBJS += \
./Core/Src/TIM_IT.o \
./Core/Src/badminton.o \
./Core/Src/baseball_intake.o \
./Core/Src/main.o \
./Core/Src/mainpp.o \
./Core/Src/reset_button.o \
./Core/Src/ros_port.o \
./Core/Src/servo.o \
./Core/Src/shooter.o \
./Core/Src/shooter_base.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o 

CPP_DEPS += \
./Core/Src/TIM_IT.d \
./Core/Src/badminton.d \
./Core/Src/baseball_intake.d \
./Core/Src/mainpp.d \
./Core/Src/reset_button.d \
./Core/Src/ros_port.d \
./Core/Src/servo.d \
./Core/Src/shooter.d \
./Core/Src/shooter_base.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.cpp Core/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/TIM_IT.d ./Core/Src/TIM_IT.o ./Core/Src/TIM_IT.su ./Core/Src/badminton.d ./Core/Src/badminton.o ./Core/Src/badminton.su ./Core/Src/baseball_intake.d ./Core/Src/baseball_intake.o ./Core/Src/baseball_intake.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/mainpp.d ./Core/Src/mainpp.o ./Core/Src/mainpp.su ./Core/Src/reset_button.d ./Core/Src/reset_button.o ./Core/Src/reset_button.su ./Core/Src/ros_port.d ./Core/Src/ros_port.o ./Core/Src/ros_port.su ./Core/Src/servo.d ./Core/Src/servo.o ./Core/Src/servo.su ./Core/Src/shooter.d ./Core/Src/shooter.o ./Core/Src/shooter.su ./Core/Src/shooter_base.d ./Core/Src/shooter_base.o ./Core/Src/shooter_base.su ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su

.PHONY: clean-Core-2f-Src

