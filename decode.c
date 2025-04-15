/**
 * @file decode.c
 * @brief Реализация всех функций декодирования
 */

#include "header.h"
#define uc unsigned char

void Decode_from_base16 (long size, unsigned char *file_base16, unsigned char *decoded_file){
    /**
     * @brief Декодирует данные из формата base16 (HEX)
     * 
     * @param size Размер закодированных данных
     * @param file_base16 Закодированные данные
     * @param decoded_file Буфер для декодированных данных
     */
    unsigned char flag = 0;
    unsigned char first;
    unsigned char second;
    int idx = 0;

    // base16 decoding 
    for (int i = 0; i < size; i += 2){
        for (int j = 0; j < 16; j++){
            if (base16[j] == file_base16[i]){
                first = j;
                flag++;
            }
            if (base16[j] == file_base16[i+1]){
                second = j;
                flag++;
            }
            if(flag == 2){
                break;
            }
        }
        if (flag == 2){
            decoded_file[idx] = (first << 4) | second;
            idx++;
        }
        //fprintf(fp_out,"%c", decode_char);
        flag = 0;
    }
}

void Decode_from_base32(int size, uc *file_base32, uc *decoded_file_from_base32){
    /**
     * @brief Декодирует данные из формата base32
     * 
     * @param size Размер закодированных данных
     * @param file_base32 Закодированные данные
     * @param decoded_file_from_base32 Буфер для декодированных данных
     */
    uint64_t temp = 0;
    uint64_t with_mask;
    int idx = 0;
    int flag = 0;
    int sub = 0;
    for(int i = 0; i < size; i += 8){
        uint64_t mask = 0b11111111;
        mask <<= 32;
        for(int j = 0; j < 8; j++){
            for(int k = 0; k < 33; k++){
                if (base32[k] == file_base32[i + j]){
                    flag = 1;
                    sub = k;
                }

            }
            if (flag){
                temp |= sub;
                temp <<= 5;
            }
        }
        temp >>= 5;
        for(int j = 0; j < 5; j++){
            with_mask = temp & mask;
            with_mask >>=  32 - (j * 8) ;
            decoded_file_from_base32[idx] = (uc)with_mask;
            idx++;
            mask >>= 8;
        }
        temp = 0;
    }
}


void Decode_from_base64(int size, uc *file_base64, uc *decoded_file_from_base64){
    /**
     * @brief Декодирует данные из формата base64
     * 
     * @param size Размер закодированных данных
     * @param file_base64 Закодированные данные
     * @param decoded_file_from_base64 Буфер для декодированных данных
     */
    uint64_t temp = 0;
    uint64_t with_mask;
    int idx = 0;
    int flag = 0;
    int sub = 0;
    for(int i = 0; i < size; i += 8){
        uint64_t mask = 0b11111111;
        mask <<= 40;
        for(int j = 0; j < 8; j++){
            for(int k = 0; k < 64; k++){
                if (base64[k] == file_base64[i + j]){
                    flag = 1;
                    sub = k;
                }

            }
            if (flag){
                temp |= sub;
                temp <<= 6;
            }
        }
        temp >>= 6;
        for(int j = 0; j < 6; j++){
            with_mask = temp & mask;
            with_mask >>=  40 - (j * 8) ;
            decoded_file_from_base64[idx] = (uc)with_mask;
            idx++;
            mask >>= 8;
        }
        temp = 0;
    }
}

void Custom_base_decode(long file_size, int MOD, uc *file_base, uc *file){
    /**
     * @brief Декодирует данные из пользовательской системы счисления на основе base64
     * 
     * @param file_size Размер декодированных данных
     * @param MOD Основание системы счисления
     * @param file_base Закодированные данные
     * @param file Буфер для декодированных данных
     */
    int idx = 0;
    for(int i = 0; i < file_size; i++){
        int temp = 0;
        for(int j = 0; j < MOD; j++){
            if(base64[j] == file_base[idx]){
                temp += j;
            }
        }
        idx++;
        for(int j = 0; j < MOD; j++){
            if(base64[j] == file_base[idx]){
                temp += j * MOD;
            }
        }
        idx++;
        file[i] = temp;
    }
}


void Decryption(int size, long file_size,uc *file, uc *file_base){
    /**
     * @brief Декодирует из пользовательской системы
     * хоть тут и написано якобы это какая то шифровка но на самом деле просто кастомная кодировка
     * 
     * @param size Размер зашифрованных данных
     * @param file_size Размер декодированных данных
     * @param file Буфер для декодированных данных
     * @param file_base Зашифрованные данные
     */
    printf("Напишите алфавит кодировки\n> ");
    uc key[200];
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = 0;
    int MOD = strlen(key);
    int temp;
    int idx = 0;
    int sub = 0;
    int pow;
    int MAX = 8;
    if (MOD > 16){
        MAX = 2;
    }
    else if (MOD <= 16 && MOD >= 8){
        MAX = 3;
    }
    for(int i = 0; i < file_size; i++){
        temp = 0;
        for(int j = 0; j < MAX; j++){
            if(j == 0){
                pow = 1;
            }
            else{
                pow *= MOD;
            }
            for(int k = 0; k < MOD; k++){
                if(key[k] == file_base[idx]){
                    idx++;
                    sub = k;
                    break;
                }
            }
            sub *= pow;
            temp += sub;
        }
        file[i] = temp;
    }
}

void Dec_URL(long file_size, uc *file, uc *file_base){
    /**
     * @brief Декодирует данные из URL-формата
     * 
     * @param file_size Размер декодированных данных
     * @param file Буфер для декодированных данных
     * @param file_base Закодированные данные
     */
    int idx = 0;
    int temp = 0;
    for(int i = 0; i < file_size; i++){
        temp = 0;
        if(file_base[idx] == '%'){
            idx++;
            for(int k = 0; k < 16; k++){
                if(file_base[idx] == base16[k]){
                    temp |= k;
                    temp <<= 4;
                    break;
                }
            }
            idx++;
            for(int k = 0; k < 16; k++){
                if(file_base[idx] == base16[k]){
                    temp |= k;
                    break;
                }
            }
            idx++;
        }
        else{
            temp = file_base[idx];
            idx++;
        }
        file[i] = temp;
    }
}

void Dec_bin(long file_size, uc *file, uc *file_base){
    /**
     * @brief Декодирует данные из бинарного представления
     * 
     * @param file_size Размер декодированных данных
     * @param file Буфер для декодированных данных
     * @param file_base Закодированные данные
     */
    int idx = 0;
    int temp;
    int sub;
    for(int i = 0; i < file_size; i++){
        sub = 0;        
        for(int j = 0; j < 8; j++){
            temp = file_base[idx] - 48;
            temp <<= 7 - j;
            idx++;
            sub += temp;
        }
        file[i] = sub;
    }
}