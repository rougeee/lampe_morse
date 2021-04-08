cmake_minimum_required(VERSION 3.10.2)
project(Guillaume C)

set(CMAKE_C_STANDARD 99)

add_executable(Guillaume main.c seven-seg.c UART.c program.c Fake_EEPROM.c)