################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/tests/tableLookupTest.cpp \
../src/tests/tests.cpp \
../src/tests/utilTest.cpp 

OBJS += \
./src/tests/tableLookupTest.o \
./src/tests/tests.o \
./src/tests/utilTest.o 

CPP_DEPS += \
./src/tests/tableLookupTest.d \
./src/tests/tests.d \
./src/tests/utilTest.d 


# Each subdirectory must supply rules for building sources it contributes
src/tests/%.o: ../src/tests/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/eric/gtest-1.6.0/include -I"/home/eric/freeEMSUnitTest/freeems-vanilla/src/main" -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


