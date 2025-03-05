#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bmp.h"
#include "playfair.h"

// Znaky, ktoré sa môžu používať
#define ALPHA "ABCDEFGHIJKLMNOPQRSTUVXYZ"

// Veľkosť matice
#define MATRIX_SIZE 5

// Funkcia na šifrovanie textu
char* playfair_encrypt(const char* key, const char* text) {
    if (key == NULL || text == NULL) return NULL;

    if ( (strlen(key) == 1) && (key[0] == ' ') ) return NULL;

    // Kontrola, či key obsahuje len znaky abecedy
    for (int i = 0; i < strlen(key); i++) {
        if ( (!isalpha(key[i])  && (key[i] != ' ')) ) {  // Ak znak nie je písmeno abecedy
            return NULL;  // Vráti NULL, ak kľúč nie je platný
        }
    }

    // Vytvorenie matice 5x5
    char matrix[MATRIX_SIZE][MATRIX_SIZE];
    int used[26] = {0};  // Pole na sledovanie použitých písmen, každý jeden prvok je písmeno, začíname A a končíme Z
    int index = 0;       // Index pre naplnenie matice

    // Naplnenie matice "matrix" kľúčom
    // index je v podstate číslo (je to počet prvkov poľa) a pomocou neho napľňame maticu 5x5, Ak požijeme index/5 je to definícia riadku, ak index%5 je to stľpec
    for (int i = 0; key[i] != '\0'; i++) {  // Prechádzame celý kľúč, až do konca
        char c = toupper(key[i]);            // Zmeníme na veľké písmeno
        if (c == ' ') continue;  // Ak je medzera, preskočíme ju
        if (c == 'W') c = 'V';   // Ak je tam 'W' zmeníme ho na 'V' - podmienka úlohy
        if (used[c - 'A'] == 0) {  // Ak písmeno ešte nebolo použité, index pola used je presne číslo daného písmena v abecede (preto -A aby sme sa dostali na pozíciu)
            matrix[index / MATRIX_SIZE][index % MATRIX_SIZE] = c;   // Vložíme do matice znak
            used[c - 'A'] = 1;  // Označíme v used, že už bolo písmeno použité
            index++;            // Na ďalší znak
        }
    }




    // Doplň zvyšok abecedy (bez 'W') pomocou ALPHA v ALPHA je 25 znakov teda 5x5 matica
    for (int i = 0; ALPHA[i] != '\0'; i++) {
        char c = ALPHA[i];          // Zoberieme písmeno
        if (used[c - 'A'] == 0) {   // Ak nebolo pouižité
            matrix[index / MATRIX_SIZE][index % MATRIX_SIZE] = c; // Vložíme do matice, všímame si, že používam,e stálke rovnaký "index", teda pokračujeme v naplnení matice 5x5
            index++;    // Ideme na ďalší prvok matice 5x5
        }
    }

    // Úprava textu pred šifrovaním
    // char* prepared_text = strdup(text);  // Skopírujeme si TEXT

    char* prepared_text= malloc(strlen(text) + 1);

    if (prepared_text != NULL) {
        strcpy(prepared_text, text);
    }

    int len = strlen(prepared_text);    // Zistíme jeho dľžku
    int j = 0;

    // Odstránenie medzier a konverzia na veľké písmená
    for (int i = 0; i < len; i++) {       // Prechádzame TEXT
        if (prepared_text[i] != ' ') {
            // i--;// Ak je tam niej emedzera
            prepared_text[j++] = toupper(prepared_text[i]);
            //j++;
            // Zmeň na veľké
        }       // Ak tam nie je medzera ide "j" a "i" v jednom súbehu ale ak je tam medzera "i" sa zvyšuje ale "j" ostáva prepared_text[j++] najprv pole[j]  potom j=j+1
    }       // Ak je tam medzera ideme na ďalší znak
    prepared_text[j] = '\0';

    // Pridanie 'X' medzi rovnaké znaky a na koniec, ak je dĺžka nepárna
    // Ak pridávame môže sa stať že sa mení prebežne dľžka aj to či je TEXT párny/nepárny
    len = strlen(prepared_text);
    for (int i = 0; i < len; i += 2) { // Robíme po dvojiciach i=i+2
        if (i + 1 < len && prepared_text[i] == prepared_text[i + 1]) { // Ak nerobíme poslednú dvojicu A ak je aktuálny aj nasledujúci znak rovnaký
            memmove(&prepared_text[i + 1], &prepared_text[i], len - i); // Tento zápis vkladá nový znak do reťazca na pozícii i tak, že posunie všetky znaky doprava
            prepared_text[i + 1] = 'X'; // Keď sme všetko posunuli o znak tak pridáme "X"
            len++; // Pridali sme znak "X" treba zvýsiť dľžku TEXTu
            prepared_text[len] = '\0';  // Na koniec pridaj ukončenie
        }
    }
    if (len % 2 != 0) {     // Ak je nepárny počet upravených znakov
        prepared_text[len] = 'X';   // Pridaj na koniec "X"
        prepared_text[len + 1] = '\0';  // A ukončovací znak
    }

    // Šifrovanie textu
    char* encrypted = (char*)malloc(len + 1); // Príprava
    if (encrypted == NULL) {
        free(prepared_text);
        return NULL;
    }
    int row_a=0, col_a=0, row_b=0, col_b=0;
    for (int i = 0; i < len; i += 2) { // Robíme po dvojiciach
        char a = prepared_text[i]; // Prvý znak dvojice je "a"
        char b = prepared_text[i + 1];  // Druhý znak dvojice je "b"

        // Nájdenie pozície znakov v matici

        // Prechádzame maticou a hľadáme pozície dvojice
        for (int r = 0; r < MATRIX_SIZE; r++) {          // Riadky
            for (int c = 0; c < MATRIX_SIZE; c++) {     // Stľpce
                if (matrix[r][c] == a) {    // Našli sme prvý znak
                    row_a = r;              // Uložíme pozície prvého znaku
                    col_a = c;
                }
                if (matrix[r][c] == b) {    // Našli sme druhý znak
                    row_b = r;              // Uložíme pozície druhého znaku
                    col_b = c;
                }
            }
        }

        // Šifrovanie dvojice znakov
        if (row_a == row_b) {  // Rovnaký riadok = berieme nasledujúcu pozíciu v riadku
            encrypted[i] = matrix[row_a][(col_a + 1) % MATRIX_SIZE];
            encrypted[i + 1] = matrix[row_b][(col_b + 1) % MATRIX_SIZE];
        } else if (col_a == col_b) {  // Rovnaký stĺpec = berieme pozíciu nižsie v stľpci
            encrypted[i] = matrix[(row_a + 1) % MATRIX_SIZE][col_a];
            encrypted[i + 1] = matrix[(row_b + 1) % MATRIX_SIZE][col_b];
        } else {  // Rôzne riadky a stĺpce
            encrypted[i] = matrix[row_a][col_b];    // Prvý znak riadok ostáva , stľpec sa berie podľa druhého znaku
            encrypted[i + 1] = matrix[row_b][col_a];    // Druhý znak to isté
        }
    
    }
    encrypted[len] = '\0';   // doplníme ukončovací znak


    // Po šifrovaní upravíme výsledok na formát po dvojiciach
    int len_double = strlen(encrypted);
    char* result = (char*)malloc(2 * len_double); // Každá dvojica má 2 znaky + medzera
    if (result == NULL) {
        free(encrypted);
        return NULL;
    }

    int j_double = 0;
    for (int i = 0; i < len_double; i += 2) {
        result[j_double++] = encrypted[i];
        result[j_double++] = encrypted[i + 1];
        if (i + 2 < len_double) {
            result[j_double++] = ' '; // Pridáme medzeru medzi dvojice
        }
    }
    result[j_double] = '\0'; // Ukončenie reťazca

    free(encrypted); // Uvoľníme pôvodný zašifrovaný reťazec
    free(prepared_text);    // uvoľníme pamäť
    return result;       // návrat zašifrovaného textu
}

// Funkcia na dešifrovanie textu porebieha opačne ako šifrovanie :-)
char* playfair_decrypt(const char* key, const char* text) {
    if (key == NULL || text == NULL) return NULL;

  if ( (strlen(key) == 1) && (key[0] == ' ') ) return NULL;     // Ak je to len jeden znak a to medzera

    // Kontrola, či key obsahuje len znaky abecedy
    for (int i = 0; i < strlen(key); i++) {
        if ( (!isalpha(key[i])  && (key[i] != ' ')) ) {  // Ak znak nie je písmeno abecedy
            return NULL;  // Vráti NULL, ak kľúč nie je platný
        }
    }

    // Vytvorenie matice 5x5
    char matrix[MATRIX_SIZE][MATRIX_SIZE];
    int used[26] = {0};  // Pole na sledovanie použitých písmen
    int index = 0;       // Index pre naplnenie matice

    // Naplnenie matice kľúčom
    for (int i = 0; key[i] != '\0'; i++) {
        char c = toupper(key[i]);
        if (c == ' ') continue;  // Ignorovať medzery
        if (c == 'W') c = 'V';   // Nahradiť 'W' za 'V'
        if (used[c - 'A'] == 0) {  // Ak písmeno ešte nebolo použité
            matrix[index / MATRIX_SIZE][index % MATRIX_SIZE] = c;
            used[c - 'A'] = 1;
            index++;
        }
    }

    // Doplň zvyšok abecedy (bez 'W') pomocou ALPHA
    for (int i = 0; ALPHA[i] != '\0'; i++) {
        char c = ALPHA[i];
        if (used[c - 'A'] == 0) {
            matrix[index / MATRIX_SIZE][index % MATRIX_SIZE] = c;
            index++;
        }
    }

    // Kontrola, či text obsahuje 'W'
    if (strchr(text, 'W') != NULL) {
        return NULL;
    }

    // Dešifrovanie textu
    int len = strlen(text);
    char* decrypted = (char*)malloc(len + 1);
    if (decrypted == NULL) {
        return NULL;
    }
    int row_a=0, col_a=0, row_b=0, col_b=0;
    for (int i = 0; i < len; i += 2) {
        char a = text[i];
        char b = text[i + 1];

        // Nájdenie pozície znakov v matici
//          int row_a, col_a, row_b, col_b;
        for (int r = 0; r < MATRIX_SIZE; r++) {
            for (int c = 0; c < MATRIX_SIZE; c++) {
                if (matrix[r][c] == a) {
                    row_a = r;
                    col_a = c;
                }
                if (matrix[r][c] == b) {
                    row_b = r;
                    col_b = c;
                }
            }
        }

        // Dešifrovanie dvojice znakov
        if (row_a == row_b) {  // Rovnaký riadok
            decrypted[i] = matrix[row_a][(col_a - 1 + MATRIX_SIZE) % MATRIX_SIZE];
            decrypted[i + 1] = matrix[row_b][(col_b - 1 + MATRIX_SIZE) % MATRIX_SIZE];
        } else if (col_a == col_b) {  // Rovnaký stĺpec
            decrypted[i] = matrix[(row_a - 1 + MATRIX_SIZE) % MATRIX_SIZE][col_a];
            decrypted[i + 1] = matrix[(row_b - 1 + MATRIX_SIZE) % MATRIX_SIZE][col_b];
        } else {  // Rôzne riadky a stĺpce
            decrypted[i] = matrix[row_a][col_b];
            decrypted[i + 1] = matrix[row_b][col_a];
        }
    }
    decrypted[len] = '\0';

    return decrypted;
}


