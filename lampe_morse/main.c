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

#ifndef PIC_VERSION
#include <stdlib.h>
#endif


// Including headers for the PC version - depends on what is found in version.h
#ifndef PIC_VERSION

#include <unistd.h>

#endif

int main() {
#ifdef PIC_VERSION
    TRISD = 0x0;
    PORTD = 0x0;
#endif

    UART_Init();
    initialisation();

    while (1) {
        char letter; //initialisation de la lettre qui va correspondre à la commande
        UART_Write_Text("Enter command (h for help): ");
        letter = UART_Read(); //retourne la valeur de la lettre
        UART_Clear_Buffer();
        UART_Write_Text("\n");

        switch (letter) {
            case 'h':
                displayCommand();
                break;

            case 'a':
                addSentence();
                break;

            case 'd':
                deleteSentence();
                break;

            case 'l':
                listAllSentences();
                break;

            case 'f':
                displayFreeData();
                break;

            case 'r':
                playDefaultSentence();
                break;

            case 's':
                setDefaultSentence();
                break;

            case 'n':
                setNextDefaultSentence();
                break;

            case 'p':
                playAllSentences();
                break;

#ifndef PIC_VERSION
            case '$':
                exit(EXIT_SUCCESS);
#endif

        }
    }


#ifndef PIC_VERSION
    return 0;
#endif
}