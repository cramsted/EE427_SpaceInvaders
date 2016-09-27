################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/globals.c \
../src/main.c \
../src/platform.c \
../src/render.c \
../src/sprite_bit_maps.c \
../src/sprite_char_bit_maps.c \
../src/sprites.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/globals.o \
./src/main.o \
./src/platform.o \
./src/render.o \
./src/sprite_bit_maps.o \
./src/sprite_char_bit_maps.o \
./src/sprites.o 

C_DEPS += \
./src/globals.d \
./src/main.d \
./src/platform.d \
./src/render.d \
./src/sprite_bit_maps.d \
./src/sprite_char_bit_maps.d \
./src/sprites.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo Building file: $<
	@echo Invoking: MicroBlaze gcc compiler
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../hello_world_bsp_0/microblaze_0/include -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.20.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '


