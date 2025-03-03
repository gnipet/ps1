#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bmp.h"
#include "playfair.h"



int main() {
    
    // Test reverse
    printf("\nSpúšťam reverse:   ");
//    printf("\nPožadovaný výstup: !DLROW OLLEH\n");
    char* reversed = reverse("Hello world!");
    printf("%s\n", reversed); 
    free(reversed);

    //Test dešifrovania
    printf("\nSpúšťam vigenere_encrypt:   ");
//    printf("\nPožadovaný výstup: JSXAI PSINR\n");
    char* encrypted = vigenere_encrypt("CoMPuTeR", "Hello world!");
    printf("%s", encrypted);  

    // Test dešifrovania
    printf("\nSpúšťam vigenere_decrypt:  ");
//    printf("\nPožadovaný výstup: HELLO WORLD!\n");
    char* decrypted = vigenere_decrypt("CoMPuTeR", "JSXAI PSINR!");
    printf("%s\n", decrypted);  
    free(decrypted);
    free(encrypted);
    
    // Test bitového šifrovania
    printf("\nSpúšťam bitový chaos - encrypt:  ");
//    printf("\nPožadovaný výstup: 80 9c 95 95 96 11 bc 96 b9 95 9d 10\n");
    unsigned char* chaos_encrypted = bit_encrypt("Hello world!");
    for(int i=0; i < 12;i++) {
    	printf("%x ", chaos_encrypted[i]); //Prirodzené hexadecimálne číslo 
    }
//    printf("\n");

    // Test bitového dešifrovania
    printf("\nSpúšťam bitový chaos - dencrypt:  ");
//    printf("\nPožadovaný výstup: Hello world!");
    char* chaos_decrypted = bit_decrypt(chaos_encrypted);
    printf("%s", chaos_decrypted);
    free(chaos_decrypted);
    free(chaos_encrypted);
    printf("\n");

    //Test dešifrovania
    printf("\nSpúšťam BMP_encrypt:   ");
    unsigned char* bmp_encrypted = bmp_encrypt("CoMPuTeR", "Hello world!");
    for(int i=0; i < 12;i++) {
        printf("%x ", bmp_encrypted[i]); //Prirodzené hexadecimálne číslo 
    }

    // Test dešifrovania
    printf("\nSpúšťam BMP_decrypt:  ");
    char* bmp_decrypted = bmp_decrypt("CoMPuTeR", bmp_encrypted);
    printf("%s\n", bmp_decrypted);
    free(bmp_decrypted);
    free(bmp_encrypted);
 
    printf("\n");

    // PLAYFAIR

    const char* fair_key = "belfast";
    const char* fair_text = "KRYPTOGRAFIA";

    // Šifrovanie
    char* fair_encrypted = playfair_encrypt(fair_key, fair_text);
    printf("Zašifrovaný text: %s\n", fair_encrypted);
    free(fair_encrypted);

    // Dešifrovanie
    const char* fair_encrypted_text = "MQXQIVMZBAME";
    char* fair_decrypted = playfair_decrypt(fair_key, fair_encrypted_text);
    printf("Dešifrovaný text: %s\n", fair_decrypted);
    free(fair_decrypted);

    printf("\n\n");

    return 0;
}
