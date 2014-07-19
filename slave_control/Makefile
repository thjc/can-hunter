F_CPU=16000000
VARIANT=standard
MCU=atmega328p
#MCU=atmega168
TARGET=slave_control

CXX=avr-g++
CC=avr-gcc

COMMON_FLAGS+= -IZumoMotors
CXX_SRC+= ZumoMotors/ZumoMotors.cpp
SRC +=

include make.arduino
include make.core

