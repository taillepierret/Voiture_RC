################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../22-008_Voiture_RC/Core/Startup/startup_stm32l010c6tx.s 

OBJS += \
./22-008_Voiture_RC/Core/Startup/startup_stm32l010c6tx.o 

S_DEPS += \
./22-008_Voiture_RC/Core/Startup/startup_stm32l010c6tx.d 


# Each subdirectory must supply rules for building sources it contributes
22-008_Voiture_RC/Core/Startup/%.o: ../22-008_Voiture_RC/Core/Startup/%.s 22-008_Voiture_RC/Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m0plus -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean-22-2d-008_Voiture_RC-2f-Core-2f-Startup

clean-22-2d-008_Voiture_RC-2f-Core-2f-Startup:
	-$(RM) ./22-008_Voiture_RC/Core/Startup/startup_stm32l010c6tx.d ./22-008_Voiture_RC/Core/Startup/startup_stm32l010c6tx.o

.PHONY: clean-22-2d-008_Voiture_RC-2f-Core-2f-Startup

