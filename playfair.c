#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bmp.h"
#include "playfair.h"


// Veľkosť matice
#define MATRIX_SIZE 5

// Funkcia na šifrovanie textu
char* playfair_encrypt(const char* key, const char* text) {
    if (key == NULL || text == NULL) return NULL;

    // Vytvorenie matice 5x5
    char matrix[MATRIX_SIZE][MATRIX_SIZE];
    int used[26] = {0};  // Pole na sledovanie použitých písmen
    int index = 0;       // Index pre naplnenie matice

    // Naplnenie matice kľúčom
    for (size_t i = 0; key[i] != '\0'; i++) {
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
    for (size_t i = 0; ALPHA[i] != '\0'; i++) {
        char c = ALPHA[i];
        if (used[c - 'A'] == 0) {
            matrix[index / MATRIX_SIZE][index % MATRIX_SIZE] = c;
            index++;
        }
    }

    // Úprava textu pred šifrovaním
    //char* prepared_text = strdup(text);
    
    char* prepared_text= malloc(strlen(text) + 1);

    if (prepared_text != NULL) {
        strcpy(prepared_text, text);
    }
    

    size_t len = strlen(prepared_text);
    size_t j = 0;

    // Odstránenie medzier a konverzia na veľké písmená
    for (size_t i = 0; i < len; i++) {
        if (prepared_text[i] != ' ') {
            prepared_text[j++] = toupper(prepared_text[i]);
        }
    }
    prepared_text[j] = '\0';

    // Pridanie 'X' medzi rovnaké znaky a na koniec, ak je dĺžka nepárna
    len = strlen(prepared_text);
    for (size_t i = 0; i < len; i += 2) {
        if (i + 1 < len && prepared_text[i] == prepared_text[i + 1]) {
            memmove(&prepared_text[i + 1], &prepared_text[i], len - i);
            prepared_text[i + 1] = 'X';
            len++;
            prepared_text[len] = '\0';
        }
    }
    if (len % 2 != 0) {
        prepared_text[len] = 'X';
        prepared_text[len + 1] = '\0';
    }

    // Šifrovanie textu
    char* encrypted = (char*)malloc(len + 1);
    if (encrypted == NULL) {
        free(prepared_text);
        return NULL;
    }

    for (size_t i = 0; i < len; i += 2) {
        char a = prepared_text[i];
        char b = prepared_text[i + 1];

        // Nájdenie pozície znakov v matici
        int row_a, col_a, row_b, col_b;
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

        // Šifrovanie dvojice znakov
        if (row_a == row_b) {  // Rovnaký riadok
            encrypted[i] = matrix[row_a][(col_a + 1) % MATRIX_SIZE];
            encrypted[i + 1] = matrix[row_b][(col_b + 1) % MATRIX_SIZE];
        } else if (col_a == col_b) {  // Rovnaký stĺpec
            encrypted[i] = matrix[(row_a + 1) % MATRIX_SIZE][col_a];
            encrypted[i + 1] = matrix[(row_b + 1) % MATRIX_SIZE][col_b];
        } else {  // Rôzne riadky a stĺpce
            encrypted[i] = matrix[row_a][col_b];
            encrypted[i + 1] = matrix[row_b][col_a];
        }
    }
    encrypted[len] = '\0';

    free(prepared_text);
    return encrypted;
}

// Funkcia na dešifrovanie textu
char* playfair_decrypt(const char* key, const char* text) {
    if (key == NULL || text == NULL) return NULL;

    // Vytvorenie matice 5x5
    char matrix[MATRIX_SIZE][MATRIX_SIZE];
    int used[26] = {0};  // Pole na sledovanie použitých písmen
    int index = 0;       // Index pre naplnenie matice

    // Naplnenie matice kľúčom
    for (size_t i = 0; key[i] != '\0'; i++) {
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
    for (size_t i = 0; ALPHA[i] != '\0'; i++) {
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
    size_t len = strlen(text);
    char* decrypted = (char*)malloc(len + 1);
    if (decrypted == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < len; i += 2) {
        char a = text[i];
        char b = text[i + 1];

        // Nájdenie pozície znakov v matici
        int row_a, col_a, row_b, col_b;
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

