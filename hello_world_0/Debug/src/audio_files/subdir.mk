################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/audio_files/alienMovement1.c \
../src/audio_files/alienMovement2.c \
../src/audio_files/alienMovement3.c \
../src/audio_files/alienMovement4.c \
../src/audio_files/alienMovemnt3.c \
../src/audio_files/audio.c \
../src/audio_files/explosionAlien.c \
../src/audio_files/tankExplosion.c \
../src/audio_files/tankFireNoise.c \
../src/audio_files/ufoNoise.c 

OBJS += \
./src/audio_files/alienMovement1.o \
./src/audio_files/alienMovement2.o \
./src/audio_files/alienMovement3.o \
./src/audio_files/alienMovement4.o \
./src/audio_files/alienMovemnt3.o \
./src/audio_files/audio.o \
./src/audio_files/explosionAlien.o \
./src/audio_files/tankExplosion.o \
./src/audio_files/tankFireNoise.o \
./src/audio_files/ufoNoise.o 

C_DEPS += \
./src/audio_files/alienMovement1.d \
./src/audio_files/alienMovement2.d \
./src/audio_files/alienMovement3.d \
./src/audio_files/alienMovement4.d \
./src/audio_files/alienMovemnt3.d \
./src/audio_files/audio.d \
./src/audio_files/explosionAlien.d \
./src/audio_files/tankExplosion.d \
./src/audio_files/tankFireNoise.d \
./src/audio_files/ufoNoise.d 


# Each subdirectory must supply rules for building sources it contributes
src/audio_files/%.o: ../src/audio_files/%.c
	@echo Building file: $<
	@echo Invoking: MicroBlaze gcc compiler
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../hello_world_bsp_0/microblaze_0/include -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.20.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '


