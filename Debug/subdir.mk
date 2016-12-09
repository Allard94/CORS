################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../main.cpp \
../observations.cpp \
../port.cpp \
../sbp.cpp 

OBJS += \
./main.o \
./observations.o \
./port.o \
./sbp.o 

CPP_DEPS += \
./main.d \
./observations.d \
./port.d \
./sbp.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/allard/Documents/CORS_Rtklib/rtklib" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


