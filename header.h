/**
 * @file header.h
 * @brief Заголовочный файл со всеми функциями кодирования/декодирования
 */

#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define CNTL_Z '\032' /* маркер конца файла в текстовых файлах DOS */
#define SLEN 81
#define uc unsigned char

extern const uc base16[17];
extern const  uc base32[33];
extern const uc base64[65];

/* Base16 functions */
void Encode_to_base16(long file_size, int *size, unsigned char *file_base16, unsigned char *file);
void Decode_from_base16(long size, unsigned char *file_base16, unsigned char *decoded_file);

/* Base32 functions */
void Encode_to_base32(int *size, int file_size, uc *file, uc *file_base32);
void Decode_from_base32(int size, uc *file_base32, uc *decoded_file_from_base32);

/* Base64 functions */
void Encode_to_base64(int *size, int file_size, uc *file, uc *file_base64);
void Decode_from_base64(int size, uc *file_base64, uc *decoded_file_from_base64);

/* Custom base functions */
void Custom_base_encode(int *size, long file_size, int MOD, uc *file_base, uc *file);
void Custom_base_decode(long file_size, int MOD, uc *file_base, uc *file);

/* Encryption functions */
int Encryption(int *size, long file_size, uc *file, uc *file_base);
void Decryption(int size, long file_size, uc *file, uc *file_base);

/* URL encoding functions */
void En_URL(int *size, long file_size, uc *file, uc *file_base);
void Dec_URL(long file_size, uc *file, uc *file_base);

/* Binary encoding functions */
void En_bin(int *size, long file_size, uc *file, uc *file_base);
void Dec_bin(long file_size, uc *file, uc *file_base);

#endif /* HEADER_H */