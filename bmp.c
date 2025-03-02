#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bmp.h"
#include "playfair.h"

// Funkcia na vráti kópiu vstupného reťazca v obrátenom poradí (UPPERCASE)
char* reverse(const char* text) {
    // Ak je vstupný reťazec NULL, vráť NULL
    if (text == NULL) return NULL;
    
    // Dĺžka vstupného reťazca
    int length = strlen(text);

    // Alokácia pamäte pre obrátený reťazec (vrátane ukončovacieho znaku '\0')
    char* reversed = (char*)malloc((length + 1) * sizeof(char));
    if (reversed == NULL) return NULL;  // Ak alokácia zlyhala, vráť NULL

    // Obrátenie reťazca a konverzia na veľké písmená
    for (int i = 0; i < length; i++) {
        reversed[i] = toupper(text[length - 1 - i]);  // Obrátenie a konverzia
    }

    // Pridanie ukončovacieho znaku '\0'
    reversed[length] = '\0';

    // Vrátenie obráteného reťazca
    return reversed;
}



// Funkcia na zašifrovanie textu pomocou Vigenèrovej šifry
char* vigenere_encrypt(const char* key, const char* text) {
    if (key == NULL || text == NULL) return NULL;

    
    // Kontrola, či key obsahuje len znaky abecedy
    for (int i = 0; key[i] != '\0'; i++) {
        if (!isalpha(key[i])) {  // Ak znak nie je písmeno abecedy
            return NULL;  // Vráti NULL, ak kľúč nie je platný
        }
    }
    
    int key_len = strlen(key);
    int text_len = strlen(text);
    char* encrypted_text = (char*)malloc((text_len + 1) * sizeof(char)); //Alokujeme miesto pre šifrovaný reťazec + 1 to je ukončenie

    if (encrypted_text == NULL) return NULL; // ak sa nepodarilo alokovať pamäť vráť NULL

    int key_index = 0; // začíname od začiatku na kľúči

    for (int i = 0; i < text_len; i++) {      //ideme toľkokrát, aká je dľžka šifrovaného reťazca
        if (isalpha(text[i])) {  //ak je to písmeno šifrujeme
            char base;    // Určenie základného znaku
            if(isupper(text[i]))
                base = 'A';      // ak sú to veľké písmená tak základ je veľké A
            else
                base = 'a';     // ak sú to malé písmená tak základ je malé a
            char key_char = toupper(key[key_index % key_len]);  // Kľúč sa konvertuje na veľké písmeno - modulo zabezpečuje rotáciu len do dľžky kľúča
            int shift = key_char - 'A';  // Výpočet posunu - posun sa odvíja od 'A'
            encrypted_text[i] = (text[i] - base + shift) % 26 + 'A';  // Zašifrovanie a konverzia na veľké písmeno (+'A'), % 26 je modulo 26 (vráti stále 0-25) +shift=šifrujem 
            key_index++;
        } else {
            encrypted_text[i] = text[i];  // Znaky iné ako písmená sa nemenia
        }
    }

    encrypted_text[text_len] = '\0';  // Pridanie ukončovacieho znaku dávame stále keď tvoríme nový text
    return encrypted_text;     //vracia šifrovaný text - smerník na pole
}

// Funkcia na dešifrovanie textu pomocou Vigenèrovej šifry
char* vigenere_decrypt(const char* key, const char* text) {
    if (key == NULL || text == NULL) return NULL;

    // Kontrola, či key obsahuje len znaky abecedy
    for (int i = 0; key[i] != '\0'; i++) {
        if (!isalpha(key[i])) {  // Ak znak nie je písmeno abecedy
            return NULL;  // Vráti NULL, ak kľúč nie je platný
        }
    }

    int key_len = strlen(key);
    int text_len = strlen(text);
    char* decrypted_text = (char*)malloc((text_len + 1) * sizeof(char));

    if (decrypted_text == NULL) return NULL;

    int key_index = 0;

    for (int i = 0; i < text_len; i++) {
        if (isalpha(text[i])) {
            char key_char = toupper(key[key_index % key_len]);  // Kľúč sa konvertuje na veľké písmeno
            int shift = key_char - 'A';  // Výpočet posunu
            decrypted_text[i] = (text[i] - 'A' - shift + 26) % 26 + 'A';  // Dešifrovanie a konverzia na veľké písmeno -shift=dešifrujem 
            key_index++;
        } else {
            decrypted_text[i] = text[i];  // Nealfabetické znaky sa nemenia
        }
    }

    decrypted_text[text_len] = '\0';  // Pridanie ukončovacieho znaku
    return decrypted_text;
}


// Funkcia na šifrovanie pomocou XOR a výmeny bitov
unsigned char* bit_encrypt(const char* text) {
    if (text == NULL) return NULL; // Kontrola vstupu

    int len = strlen(text);
    unsigned char* encrypted = (unsigned char*)malloc(len);
    if (encrypted == NULL) return NULL;

    for (int i = 0; i < len; i++) {
        unsigned char c = text[i];   // znak z poľa text (na pozícii i)  do  c ako nezáporné číslo 0-255
        unsigned char first = (c >> 4) & 0x0F; // posunie horne 4 bity dole (c >> 4) a vynásobí 00001111 - dostaneme horné 4 bity 
        unsigned char second = c & 0x0F;       // vynásobí základ 00001111 - dostaneme dolné 4 bity

        // Výmena dvojíc bitov
        first = ((first & 0x01) << 1) | ((first & 0x02) >> 1) | ((first & 0x04) << 1) | ((first & 0x08) >> 1);
        unsigned char result = (second ^ first) | (first << 4);

        encrypted[i] = result;
    }
    return encrypted;
}

// Funkcia na dešifrovanie
char* bit_decrypt(const unsigned char* text) {
    if (text == NULL) return NULL; // Kontrola vstupu

    int len = 0;
    while (text[len] != '\0') {
        len++;
    }

    char* decrypted = (char*)malloc(len + 1);
    if (decrypted == NULL) return NULL;

    for (int i = 0; i < len; i++) {
        unsigned char c = text[i];
        unsigned char first = (c >> 4) & 0x0F; // Prvých 4 bity
        unsigned char second = c & 0x0F;       // Druhých 4 bity

        // XOR na získanie pôvodnej druhej polovice
        unsigned char original_second = second ^ first;

        // Výmena dvojíc bitov naspäť
        first = ((first & 0x01) << 1) | ((first & 0x02) >> 1) | ((first & 0x04) << 1) | ((first & 0x08) >> 1);

        decrypted[i] = (first << 4) | original_second;
    }
    decrypted[len] = '\0'; // pridanie ukončovacieho znaku
    return decrypted;
}


unsigned char* bmp_encrypt(const char* key, const char* text) {
    if (key == NULL || text == NULL) return NULL;

    // Kontrola, či key obsahuje len znaky abecedy
    for (int i = 0; key[i] != '\0'; i++) {
        if (!isalpha(key[i])) {  // Ak znak nie je písmeno abecedy
            return NULL;  // Vráti NULL, ak kľúč nie je platný
        }
    }
        
    // 1. Obrátenie reťazca + uppercase
    char* reversed = reverse(text);
    if (reversed == NULL) return NULL;

    // 2. Vigenere šifrovanie
    char* vigenere = vigenere_encrypt(key, reversed);
    free(reversed);
    if (vigenere == NULL) return NULL;

    // 3. Bit šifrovanie
    unsigned char* encrypted = bit_encrypt(vigenere);
    free(vigenere);
    if (encrypted == NULL) return NULL;

    return encrypted;
}

char* bmp_decrypt(const char* key, const unsigned char* text) {
    if (key == NULL || text == NULL) return NULL;

    // Kontrola, či key obsahuje len znaky abecedy
    for (int i = 0; key[i] != '\0'; i++) {
        if (!isalpha(key[i])) {  // Ak znak nie je písmeno abecedy
            return NULL;  // Vráti NULL, ak kľúč nie je platný
        }
    }

    // 1. Bit dešifrovanie
    char* bit = bit_decrypt(text);
    if (bit == NULL) return NULL;

    // 2. Vigenere dešifrovanie
    char* vigenere = vigenere_decrypt(key, bit);
    free(bit);
    if (vigenere == NULL) return NULL;

    // 3. Obrátenie reťazca naspäť
    char* reversed = reverse(vigenere);
    free(vigenere);
    if (reversed == NULL) return NULL;

    return reversed;
}


