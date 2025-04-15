/**
 * @file encoding.c
 * @brief Реализация всех функций кодирования
 */

#include "header.h"

#define uc unsigned char

const uc base16[17] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

const uc base32[33] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',
                           'Q','R','S','T','U','V','W','X','Y','Z','2','3','4','5','6','7'};

const uc base64[65] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',
                           'Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f',
                           'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v',
                           'w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/'};

void Encode_to_base16(long file_size, int *size, unsigned char *file_base16, unsigned char *file) {
    /**
     * @brief Кодирует данные в формат base16 (HEX).
     * 
     * @param file_size Размер исходного файла в байтах
     * @param size Указатель на переменную для размера закодированных данных
     * @param file_base16 Буфер для закодированных данных
     * @param file Исходные данные
     */
    int idx = 0;
    for (int i = 0; i < file_size; i++){
        file_base16[idx] = (file[i] & 0b11110000) >> 4;
        file_base16[idx] = base16[file_base16[idx]];
        idx++;
        file_base16[idx] = (file[i] & 0b00001111);
        file_base16[idx] = base16[file_base16[idx]];
        idx++;
    }
    *size = idx;
}

void Encode_to_base32(int *size, int file_size, uc *file, uc *file_base32){
    /**
     * @brief Кодирует данные в формат base32
     * 
     * @param size Указатель на переменную для размера закодированных данных
     * @param file_size Размер исходных данных
     * @param file Исходные данные
     * @param file_base32 Буфер для закодированных данных
     */
    int idx = 0;
    uint64_t with_mask = 0;
    uc current_bit = 0;         // Счетчик битов в текущем байте
    uc current_byte = 0;        // Индекс текущего байта
    uint64_t temp = 0;
    uint64_t mask;
    int count = 0;
    int sub = 0;
    int bit_count = 0;
    for(int i = 0; i < file_size; i+= 5){
        if (file_size - i >= 5){
            mask = 0b11111000;
            mask <<= 32;
            for (int j = 0; j < 5; j++){
                temp |= file[i + j];
                temp <<= 8;
            }
            temp >>= 8;
            for(int j = 0; j < 8; j++){
                with_mask = temp & mask;
                with_mask >>=  35 - (j * 5);
                file_base32[idx] = base32[with_mask];
                idx++;
                mask >>= 5;
            }
            temp = 0;
        }
        else{
            temp = 0;
            sub = file_size - i;
            bit_count = sub * 8;
            for(int j = i; j < file_size; j++){
                temp |= file[j];
                temp <<= 8;
            }
            temp >>= 8;
            while (bit_count % 5 != 0){
                temp <<= 1;
                bit_count++;
            }
            mask = 0b11111000;
            mask <<= bit_count - 8;
            for(int j = 0; j < (bit_count / 5); j++){
                with_mask = temp & mask;
                with_mask >>=  bit_count - 5 - (j * 5);
                file_base32[idx] = base32[with_mask];
                idx++;
                mask >>= 5;
            }
        }
    }
    idx--;
    while(idx % 8 != 0){
        idx++;
        file_base32[idx] = '=';
    }
    *size = idx;
}

void Encode_to_base64(int *size, int file_size, uc *file, uc *file_base64){
    /**
     * @brief Кодирует данные в формат base64
     * 
     * @param size Указатель на переменную для размера закодированных данных
     * @param file_size Размер исходных данных
     * @param file Исходные данные
     * @param file_base64 Буфер для закодированных данных
     */
    int idx = 0;
    uint64_t with_mask = 0;
    uc current_bit = 0;         // Счетчик битов в текущем байте
    uc current_byte = 0;        // Индекс текущего байта
    uint64_t temp = 0;
    uint64_t mask;
    int count = 0;
    int sub = 0;
    int bit_count = 0;
    for(int i = 0; i < file_size; i += 6){
        if (file_size - i >= 6){
            mask = 0b11111100;
            mask <<= 40;
            for (int j = 0; j < 6; j++){
                temp |= file[i + j];
                temp <<= 8;
            }
            temp >>= 8;
            for(int j = 0; j < 8; j++){
                with_mask = temp & mask;
                with_mask >>=  42 - (j * 6);
                file_base64[idx] = base64[with_mask];
                idx++;
                mask >>= 6;
            }
            temp = 0;
        }
        else{
            temp = 0;
            sub = file_size - i;
            bit_count = sub * 8;
            for(int j = i; j < file_size; j++){
                temp |= file[j];
                temp <<= 8;
            }
            temp >>= 8;
            while (bit_count % 6 != 0){
                temp <<= 1;
                bit_count++;
            }
            mask = 0b11111100;
            mask <<= bit_count - 8;
            for(int j = 0; j < (bit_count / 6); j++){
                with_mask = temp & mask;
                with_mask >>=  bit_count - 6 - (j * 6);
                file_base64[idx] = base64[with_mask];
                idx++;
                mask >>= 6;
            }
        }
    }
    idx--;
    while(idx % 8 != 0){
        idx++;
        file_base64[idx] = '=';
    }
    *size = idx;
}

void Custom_base_encode(int *size, long file_size, int MOD, uc *file_base, uc *file){
    /**
     * @brief Кодирует данные в пользовательскую систему счисления на основе base64
     * 
     * @param size Указатель на переменную для размера закодированных данных
     * @param file_size Размер исходных данных
     * @param MOD Основание системы счисления
     * @param file_base Буфер для закодированных данных
     * @param file Исходные данные
     */
    for(int i = 0; i < file_size; i++){
        file_base[*size] = base64[file[i] % MOD];
        (*size)++;
        file_base[*size] = base64[file[i] / MOD];
        (*size)++;
    }
}

int Encryption(int *size, long file_size, uc *file, uc *file_base){
    /**
     * @brief Кодирует в пользовательскую систему
     * хоть тут и написано якобы это какая то шифровка но на самом деле просто кастомная кодировка
     * 
     * @param size Указатель на переменную для размера зашифрованных данных
     * @param file_size Размер исходных данных
     * @param file Исходные данные
     * @param file_base Буфер для зашифрованных данных
     * @return int Длина ключа
     */
    printf("Напишите алфовит кодирования\n> ");
    uc key[200];
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = 0;
    int MOD = strlen(key);
    uc temp = 0;
    int MAX = 8;
    if (MOD > 16){
        MAX = 2;
    }
    else if (MOD <= 16 && MOD >= 8){
        MAX = 3;
    }
    for(int i = 0; i < file_size; i++){
        temp = file[i];
        for(int j = 0; j < MAX; j++){
            file_base[*size] = key[temp % MOD];
            temp /= MOD;
            (*size)++;
        }
    }
    return MOD;
}

void En_URL(int *size, long file_size, uc *file, uc *file_base){
    /**
     * @brief Кодирует данные в URL-формат
     * 
     * @param size Указатель на переменную для размера закодированных данных
     * @param file_size Размер исходных данных
     * @param file Исходные данные
     * @param file_base Буфер для закодированных данных
     */
    int idx = 0;
    for (int i = 0; i < file_size; i++){
        if ((file[i] <= 'Z' && file[i] >= 'A') || (file[i] <= '9' && file[i] >= '0') ||
            (file[i] <= 'z' && file[i] >= 'a') || (file[i] == '-') || (file[i] == '_') ||
            (file[i] == '.') || (file[i] == '~'))
        {
            file_base[idx] = file[i];
            idx++;

        }
        else {
            file_base[idx] = '%';
            idx++;
            file_base[idx] = (file[i] & 0b11110000) >> 4;
            file_base[idx] = base16[file_base[idx]];
            idx++;
            file_base[idx] = (file[i] & 0b00001111);
            file_base[idx] = base16[file_base[idx]];
            idx++;
        }
    }
    *size = idx;
}

void En_bin(int *size, long file_size, uc *file, uc *file_base){
    /**
     * @brief Кодирует данные в бинарное представление
     * 
     * @param size Указатель на переменную для размера закодированных данных
     * @param file_size Размер исходных данных
     * @param file Исходные данные
     * @param file_base Буфер для закодированных данных
     */
    int mask = 0b10000000;
    int bit;
    for (int i = 0; i < file_size; i++){
        mask = 0b10000000;
        for(int j = 0; j < 8; j++){
            bit = 0;
            bit = file[i] & mask;
            bit >>= 7 - j;
            file_base[*size] = bit + 48;
            (*size)++;
            mask >>= 1;
        }   
    }
}