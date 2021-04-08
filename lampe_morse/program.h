#ifndef PROGRAM_H
#define PROGRAM_H

void initialisation(void); //initialise le format de données de l'EEPROM
void intToChar(int value, char* number); //convertis un entier de l'EEPROM en string
void displayCommand(void); // affiche la liste des commandes disponibles
void insertWordInTable(char* array); //insère le nouveau mot dans le tableau
void simplifySentence(char* word); //simplifie la phrase en mettant les lettres en majuscule, sipprimant les espaces en trop et les caractères spéciaux
void addSentence(void); // ajoute la phrase que l’on va rentrer dans le terminal, dans la mémoire de forme EEPROM.
void deleteSentence(void); // supprime la cellule qui contient le message avec une réorganisation automatique des cellules pour éviter des cellules vides entre 2 messages
char getChar(unsigned short valueEEPROM, unsigned short nbBits);
void convertBinaryToChar(unsigned short binary, int nbBit, char* tab); //convertis un nombre décimal en séqience binaire sous forme texte
void listAllSentences(void); // affiche toutes les phrases qui sont présentes dans la mémoire de la forme EEPROM
void displayFreeData(void); // affiche le nombre de cellules disponibles et donc le nombre de messages qu’il nous ai possible d’enregistrer
void playDefaultSentence(void); //  lis la phrase par défaut.
void setDefaultSentence(void); // définis la nouvelle phrase par défaut
void setNextDefaultSentence(void); // définis la phrase suivante comme nouvelle phrase par défaut
void playAllSentences(void);
void blinkLed(char letter);


#endif //PROGRAM_H
