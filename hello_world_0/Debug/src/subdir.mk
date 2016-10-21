################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/aliens.c \
../src/bullets.c \
../src/bunkers.c \
../src/controls.c \
../src/events.c \
../src/main.c \
../src/platform.c \
../src/render.c \
../src/sprite_bit_maps.c \
../src/sprite_char_bit_maps.c \
../src/sprites.c \
../src/tank.c \
../src/text.c \
../src/timer.c \
../src/ufo.c \
../src/xac97_l.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/aliens.o \
./src/bullets.o \
./src/bunkers.o \
./src/controls.o \
./src/events.o \
./src/main.o \
./src/platform.o \
./src/render.o \
./src/sprite_bit_maps.o \
./src/sprite_char_bit_maps.o \
./src/sprites.o \
./src/tank.o \
./src/text.o \
./src/timer.o \
./src/ufo.o \
./src/xac97_l.o 

C_DEPS += \
./src/aliens.d \
./src/bullets.d \
./src/bunkers.d \
./src/controls.d \
./src/events.d \
./src/main.d \
./src/platform.d \
./src/render.d \
./src/sprite_bit_maps.d \
./src/sprite_char_bit_maps.d \
./src/sprites.d \
./src/tank.d \
./src/text.d \
./src/timer.d \
./src/ufo.d \
./src/xac97_l.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo Building file: $<
	@echo Invoking: MicroBlaze gcc compiler
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../hello_world_bsp_0/microblaze_0/include -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.20.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '


