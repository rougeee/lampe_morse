#include "version.h"
#include "seven-seg.h"
#include "UART.h"
#include "program.h"

// Including headers for the PIC version - depends on what is found in version.h
#ifdef PIC_VERSION
#include "pragma.h"
#include <xc.h>  // Needed by the MicroChip compiler
#define _XTAL_FREQ 20000000 // Define the MCU Frequency ; needed by the MicroChip compiler
#endif

char digit[10] = {0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b00000111,
                0b01111111, 0b01101111};

char alphab[26] = {0b01110111,0b01111100,0b00111001,0b01011110,0b01111001,0b01110001,
                 0b00111101,0b01110100,0b00000100,0b00011110,0b01110101,0b00111000,
                 0b01010101,0b01010100,0b01011100,0b01110011,0b01100111,0b01010000,
                 0b01101101,0b01111000,0b00111110,0b00011100,0b01101010,0b01110110,
                 0b01101110,0b01011011};

void physical_Display(char symbol) {
    char segword;
    if ((symbol > 47) && (symbol < 58)) {
        segword = digit[symbol - 48];
    } else if ((symbol > 64) && (symbol < 91)){
        segword = alphab[symbol - 65];
    } else
        segword = 0b01000000;
#ifdef PIC_VERSION
    PORTD = segword;
#endif
}

void display_7SEG(char val, unsigned short mode) {
    if (mode == LED_ONLY || mode == UART_LED) {
        physical_Display(val);
    }
    if (mode == UART_ONLY || mode == UART_LED){
        UART_Write(val);
        UART_Write('\n');
    }
}

