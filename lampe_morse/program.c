#include "version.h"
#include "seven-seg.h"
#include "UART.h"
#include "program.h"

#ifdef PIC_VERSION
#include "pragma.h"
#include <xc.h>  // Needed by the MicroChip compiler
#define _XTAL_FREQ 20000000 // Define the MCU Frequency ; needed by the MicroChip compiler
#endif

#ifndef PIC_VERSION
#include "Fake_EEPROM.h"
#include <stdio.h>
#endif

//int letterBinary[36] = {01, 1000, 1010, 100, 0, 0010, 110, 0000, 00, 0111, 101, 0100, 11, 10, 111, 0110, 1101, 010, 000, 1, 001, 0001, 011, 1001, 1011, 1100};
unsigned char letterBinary[26] = {0b00000010, 0b00000001, 0b00000101, 0b00000001, 0b00000000, 0b00000100, 0b00000011, 0b00000000, 0b00000000, 0b00001110, 0b00000101, 0b00000010, 0b00000011, 0b00000001, 0b00000111, 0b00000110, 0b00001011, 0b00000010, 0b00000000, 0b00000001, 0b00000100, 0b00001000, 0b00000110, 0b00001001, 0b00001101, 0b00000011};
short valueOctL[26] = {2, 4, 4, 3, 1, 4, 3, 4, 2, 4, 3, 4, 2, 2, 3, 4, 4, 3, 3, 1, 3, 4, 3, 4, 4, 4};

//int numberBinary[10] = {01111, 00111, 00011, 00001, 00000, 10000, 11000, 11100, 11110, 11111};
unsigned char numberBinary[10] = {0b00011111, 0b00011110, 0b00011100, 0b00011000, 0b00010000, 0b00000000, 0b00000001, 0b00000011, 0b00000111, 0b00001111};
short valueOctN = 5;

unsigned char space = 0b00000000;
short spaceBits = 0;

void initialisation() {
    eeprom_write(0, 1); //l'EEPROM a été initialisée
    eeprom_write(1, 0);
    eeprom_write(2, 232); //nb de cellules libres
    eeprom_write(3, 0);
}


void intToChar(int value, char number[]) {
    char tab[] = "0123456789";
    int count = 0;
    int tmp = value;

    do {
        value = value/10;
        count++;
    } while (value != 0);

    for (short i = count-1; i >= 0 ; i--) {
        number[i] = tab[tmp%10];
        tmp = tmp/10;
    }

    number[count] = '\0';
}

void displayCommand() {
    UART_Write_Text("a: add a phrase\n");
    UART_Write_Text("d: delete a phrase\n");
    UART_Write_Text("l: list all phrases\n");
    UART_Write_Text("f: display the amount of free data cells\n");
    UART_Write_Text("r: play the default phrase\n");
    UART_Write_Text("s: set the default phrase\n");
    UART_Write_Text("n: set next phrase as default\n");
    UART_Write_Text("p: play all phrases\n");
#ifndef PIC_VERSION
    UART_Write_Text("$: stop the program\n\n");

    for (short i = 24; i < 40; i+=2) {
        UART_Write(getChar(eeprom_read(i),eeprom_read(i+1)));
        UART_Write_Text("\n");
    }
#endif
}

void insertWordInTable(char array[]) {
    int sizeWord = 0;
    int tmpIndex = 2 * (eeprom_read(1)+1) + 4;
    char tab[] = "";
    int indexTable = 0;
    int h = 0;

    while (array[sizeWord] != '\0') {
        sizeWord++;
    }

    if(eeprom_read(1) == 0) {
        eeprom_write(tmpIndex, 24);
        eeprom_write(tmpIndex+1, sizeWord);
        eeprom_write(3, 1);
    } else {
        eeprom_write(tmpIndex, eeprom_read(tmpIndex-2) + 2*eeprom_read(tmpIndex-1));
        eeprom_write(tmpIndex+1, sizeWord);
    }

    eeprom_write(1, eeprom_read(1) + 1);
    eeprom_write(2, eeprom_read(2) - 2*sizeWord);

    for (short k = 0; k < 2*sizeWord; k+=2) {
        if(array[indexTable] >= 'A' && array[indexTable] <= 'Z') {
            eeprom_write(eeprom_read(tmpIndex)+k, letterBinary[array[indexTable]-65]);
            eeprom_write(eeprom_read(tmpIndex)+k+1, valueOctL[array[indexTable]-65]);
        }
        if(array[indexTable] >= '0' && array[indexTable] <= '9') {
            eeprom_write(eeprom_read(tmpIndex)+k, numberBinary[array[indexTable]-48]);
            eeprom_write(eeprom_read(tmpIndex)+k+1, valueOctN);
        }
        if(array[indexTable] == ' ') {
            eeprom_write(eeprom_read(tmpIndex)+k, space);
            eeprom_write(eeprom_read(tmpIndex)+k+1, spaceBits);
        }
        indexTable++;
    }

    UART_Write_Text("Adding phrase ");
    UART_Write_Text(array);
    UART_Write_Text(" in position ");

    intToChar(eeprom_read(1),tab);
    UART_Write_Text(tab);

    UART_Write_Text("\nvalue 24: ");
    tab[0] = '\0';
    intToChar(eeprom_read(24),tab);
    UART_Write_Text(tab);

    UART_Write_Text("\nvalue 25: ");
    tab[0] = '\0';
    intToChar(eeprom_read(25),tab);
    UART_Write_Text(tab);

    UART_Write_Text("\nchar  24: ");
    UART_Write(getChar(eeprom_read(24),eeprom_read(25)));

    UART_Write_Text("\nvalue 26: ");
    tab[0] = '\0';
    intToChar(eeprom_read(26),tab);
    UART_Write_Text(tab);

    UART_Write_Text("\nvalue 27: ");
    tab[0] = '\0';
    intToChar(eeprom_read(27),tab);
    UART_Write_Text(tab);

    UART_Write_Text("\nvalue 28: ");
    tab[0] = '\0';
    intToChar(eeprom_read(28),tab);
    UART_Write_Text(tab);

    UART_Write_Text("\nvalue 29: ");
    tab[0] = '\0';
    intToChar(eeprom_read(29),tab);
    UART_Write_Text(tab);

    UART_Write_Text("\n");
}

void simplifySentence(char* word) {
    int i = 0;

    while(word[i] != '\0') {
        if(word[i] == ' ') {
            word[i] = word[i];

            if(word[i+1] == ' ') {
                int j = i+1;
                do {
                    word[j] = word[j+1];
                    j++;
                } while(word[j-1] != '\0');
                i--;
            }
        }

        else if(word[i] >= 'a' && word[i] <= 'z') {
            word[i] -= 32;
        }

        else if(word[i] >= 'A' && word[i] <= 'Z') {
            word[i] = word[i];
        }
        else if(word[i] >= '0' && word[i] <= '9') {
            word[i] = word[i];
        }
        else {
            int j = i;
            do {
                word[j] = word[j+1];
                j++;
            } while(word[j-1] != '\0');
            i--;
        }
        i++;
    }
}


void addSentence() {
    int sizeMax = eeprom_read(2)/2;
    char word[32];

    if(eeprom_read(2) <= 0) {
        UART_Write_Text("Add error: could not save new phrase\n");
        return;
    }
    if(eeprom_read(2) == 0) {
        UART_Write_Text("No definition in EEPROM\n");
    }

    UART_Write_Text("Please input a new phrase: ");
    UART_Read_Text(word, sizeMax);

    simplifySentence(word);

    if(word[0] == ' ' || word[0] == '\n') {
        UART_Write_Text("Can’t store Empty String\n");
        UART_Write_Text("Add error: could not save new phrase\n");
        return;
    } else {
        insertWordInTable(word);
    }

}

void deleteSentence() {
    char value;
    short index;

    if(eeprom_read(1) == 0) {
        UART_Write_Text("No phrase stored\n");
        return;
    } else {
        UART_Write_Text("Entry to delete: ");
        value = UART_Read();
        UART_Clear_Buffer();

        index = (short)((value) - 48);

        if(value >= 49 && value <= 57) {
            int tmpIndex = eeprom_read((2*index)+4);
            int tmpNbCarac = eeprom_read((2*index)+5);
            int sizeIndexZone = 2*eeprom_read(1)+4;

            if(index > eeprom_read(1)) {
                UART_Write_Text("Phrase doesn’t exist\n\n");
            } else {
                eeprom_write(1, eeprom_read(1)-1);

                for(int i = (2*index)+4; i <= sizeIndexZone; i+=2) {
                    eeprom_write(i, eeprom_read(i+2)-tmpNbCarac);
                    eeprom_write(i+1, eeprom_read(i+3));
                }

                for(int i = tmpIndex; i < 256; i++) {
                    eeprom_write(i, eeprom_read(tmpNbCarac+i));
                }
                eeprom_write(sizeIndexZone, 0);
                eeprom_write(sizeIndexZone+1, 0);
                eeprom_write(2, eeprom_read(2)+2*tmpNbCarac);

                UART_Write_Text("Entry deleted\n\n");
            }

        } else {
            UART_Write_Text("Invalid entry\n");
        }
    }
}

char getChar(unsigned short valueEEPROM, unsigned short nbBits) {

    if((nbBits == spaceBits) && (valueEEPROM == space)) {
        return ' ';
    }
    if(nbBits == valueOctN) {
        for (short i = 0; i < 9; ++i) {
            if(valueEEPROM == numberBinary[i]) {
                return (char)(i+48);
            }
        }
    }
    for (short j = 0; j < 26; ++j) {
        if((nbBits == valueOctL[j]) &&(valueEEPROM == letterBinary[j])) {
            return (char)(j+65);;
        }
    }
}

void convertBinaryToChar(unsigned short binary, int nbBit, char* tab) {
    /*
    int reste = 0;
    char tmp[] = "";

    for (int j = nbBit - 1; j >= 0; j--) {
        reste = binary % 2;

        intToChar(reste, tmp);
        tab[j] = tmp[0];

        tmp[0] = '\0';
        binary = binary / 2;
    }


    unsigned char mask = 0b00000001;
    //tab[8];
    for (int i=nbBit-1; i >= 0; i--) {
        tab[i] = ((binary & mask) != 0);
        mask <<= 1;
        //printf("tabBits[%d]=%d\t", i, tab[i]);

    }
    tab[nbBit]='\0';
*/
}

void listAllSentences() {
    char tab[] = "";
    int indexZone = 0;
    int readBegin = 0;
    int readEnd = 0;

    if(eeprom_read(1) == 0) {
        UART_Write_Text("No phrase stored\n");
    } else {
        for (short i = 0; i < eeprom_read(1); ++i) {
            UART_Write_Text("Phrase #");
            intToChar(i+1, tab);
            UART_Write_Text(tab);
            UART_Write_Text("\n");
            tab[0] = '\0';

            indexZone = 2*(i+1)+4;
            readBegin = eeprom_read(indexZone);
            readEnd = readBegin + 2*eeprom_read(indexZone+1);

            for (short j = readBegin; j < readEnd; j+=2) {
                if(getChar(eeprom_read(j),eeprom_read(j+1)) == ' '){
                    UART_Write_Text("(-)..");
                } else {
                    UART_Write_Text("(");
                    UART_Write(getChar(eeprom_read(j),eeprom_read(j+1)));
                    UART_Write_Text(")");

                    convertBinaryToChar(eeprom_read(j), eeprom_read(j+1), tab);

                    UART_Write_Text(".");
                }
            }
            UART_Write_Text("\n");
        }
    }
}

void displayFreeData() {
    char tab[] = "";
    UART_Write_Text("Actual free cell : ");
    intToChar(eeprom_read(2),tab);
    UART_Write_Text(tab);
    UART_Write_Text("\n");
}

void playDefaultSentence() {
    char tab[] = "";
    int indexZone = 0;
    int readBegin = 0;
    int readEnd = 0;

    if(eeprom_read(1) == 0) {
        UART_Write_Text("No phrase stored\n");
    } else {
        UART_Write_Text("Playing default phrase : ");
        intToChar(eeprom_read(3),tab);
        UART_Write_Text(tab);
        UART_Write_Text("\n");

        indexZone = 2*eeprom_read(3)+4;
        readBegin = eeprom_read(indexZone);
        readEnd = readBegin + 2*eeprom_read(indexZone+1);

        for (short j = (short)readBegin; j < readEnd; j+=2) {
            if(getChar(eeprom_read(j),eeprom_read(j+1)) == ' '){
                display_7SEG(' ', UART_LED);
            } else {
                // sequence binaires sous forme char*
                display_7SEG(getChar(eeprom_read(j),eeprom_read(j+1)), LED_ONLY);
                display_7SEG(getChar(eeprom_read(j),eeprom_read(j+1)), UART_LED);
#ifdef PIC_VERSION
                __delay_ms(1000);
#endif
                blinkLed(getChar(eeprom_read(j),eeprom_read(j+1)));

            }
#ifdef PIC_VERSION
            __delay_ms(3000);
#endif
        }
        UART_Write_Text("\n");
    }
}

void setDefaultSentence() {
    char value;
    short index;

    if(eeprom_read(1) == 0) {
        UART_Write_Text("No phrase stored\n");
        return;
    } else {
        UART_Write_Text("Entry to set as default: ");
        value = UART_Read();
        UART_Clear_Buffer();

        index = (short)((value) - 48);

        if(value >= 49 && value <= 57) {

            if(index > eeprom_read(1)) {
                UART_Write_Text("Phrase doesn’t exist\n\n");
            } else {
                eeprom_write(3, index);
                UART_Write_Text("Default entry is now : ");
                UART_Write(value);
                UART_Write_Text("\n");
            }
        } else {
            UART_Write_Text("Invalid entry\n");
        }
    }
}

void setNextDefaultSentence() {
    int index = eeprom_read(3);

    if(eeprom_read(1) == 0) {
        UART_Write_Text("No phrase stored\n");
        return;
    } else {

        index = +1;
        if (index > 9 || index > eeprom_read(1)) {
            index = 1;
        }
        eeprom_write(3, index);
    }

}

void playAllSentences() {
    char tab[] = "";
    int indexZone = 0;
    int readBegin = 0;
    int readEnd = 0;

    if(eeprom_read(1) == 0) {
        UART_Write_Text("No phrase stored\n");
    } else {
        for (short i = 0; i < eeprom_read(1); ++i) {
            indexZone = 2 * eeprom_read(3) + 4;
            readBegin = eeprom_read(indexZone);
            readEnd = readBegin + 2 * eeprom_read(indexZone + 1);

            for (short j = readBegin; j < readEnd; j += 2) {
                if (getChar(eeprom_read(j), eeprom_read(j + 1)) == ' ') {
                    display_7SEG(' ', UART_LED);
                } else {
                    // sequence binaires sous forme char*
                    display_7SEG(getChar(eeprom_read(j),eeprom_read(j+1)), LED_ONLY);
                    display_7SEG(getChar(eeprom_read(j), eeprom_read(j + 1)), UART_LED);
#ifdef PIC_VERSION
                    __delay_ms(1000);
#endif
                    blinkLed(getChar(eeprom_read(j),eeprom_read(j+1)));
                }
#ifdef PIC_VERSION
                __delay_ms(3000);
#endif
            }
#ifdef PIC_VERSION
            __delay_ms(5000);
#endif
        }
    }
}

void blinkLed(char letter) {
#ifdef PIC_VERSION
    TRISC0 = 0;
#endif

    unsigned char mask = 0b00000001;

    short entier;
    entier = letter -'A';
    short tabBits[8];//le tableau fait avec les bits
    short bit = letterBinary[entier];
    for (int i = 1 ; i <= valueOctL[entier]; i++) {
        tabBits[i] = ((bit & mask) != 0);
        mask <<= 1;
        //tabBits[i] = ((bit) != 0);
        //inverser tableau ici


        if(tabBits[i]==1){
#ifdef PIC_VERSION
            RC0 = 1; // LED ON
                __delay_ms(1000); // 4 second delay
                RC0 = 0; //LED OFF
                __delay_ms(400); // 1 second delay
#endif
        }
        if(tabBits[i]==0){
#ifdef PIC_VERSION
            RC0 = 1; // LED ON
                __delay_ms(1000); // 4 second delay
                RC0 = 0; //LED OFF
                __delay_ms(1000); // 1 second delay
#endif
        }
    }
}