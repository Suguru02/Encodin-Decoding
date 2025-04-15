#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include "header.h"

#define SLEN 81
#define uc unsigned char

int int_input(){
    /**
     * @brief Запрашивает у пользователя целое число
     * 
     * @return int Введенное пользователем число
     */
    printf("> ");
    char input[20];
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0; 
    for (int i = 0; input[i]; ++i) {
        if (!isdigit(input[i])) {
            printf("Неверный формат ввода. Введите целое число.\n");
            continue; 
        }
    }
    return atoi(input);
}

unsigned char* read_file(long *file_size){
    /**
     * @brief Читает содержимое файла
     * 
     * @param file_size Указатель на переменную для размера файла
     * @return unsigned char* Указатель на считанные данные
     */
    FILE *fp_in = NULL;
    while(!fp_in){
        printf("Введите название файла\n> ");
        char input_filename[SLEN];
        fgets(input_filename, sizeof(input_filename), stdin);
        int size = strlen(input_filename);
        char filename[SLEN] = "test.txt";
        input_filename[strcspn(input_filename, "\n")] = 0; // Удаляем символ новой строки
        fp_in = fopen(input_filename, "rb"); // Открываем файл
        if(!fp_in){
            printf("Введите корректное название\n");
        }
    }
    fseek(fp_in, 0, SEEK_END);
    *file_size = ftell(fp_in);
    rewind(fp_in); // Возвращаемся в начало файла
    uc *file = malloc(*file_size * sizeof(uc));
    fread(file, sizeof(uc), *file_size, fp_in);
    fclose(fp_in);
    return file;
}

void Write_file(uc *file, long file_size){
    /**
     * @brief Записывает данные в файл
     * 
     * @param file Данные для записи
     * @param file_size Размер данных
     */
    FILE *fp_out = NULL;
    while(!fp_out){
        printf("Введите название файла\n> ");
        char input_filename[SLEN];
        fgets(input_filename, sizeof(input_filename), stdin);
        int size = strlen(input_filename);
        char filename[SLEN] = "output.txt";
        input_filename[strcspn(input_filename, "\n")] = 0; // Удаляем символ новой строки
        fp_out = fopen(input_filename, "wb"); // Открываем файл
        if(!fp_out){
            printf("Введите корректное название\n");
        }
    }
    fwrite(file, sizeof(uc), file_size, fp_out);
    fclose(fp_out);
}

int main(void) {
    /**
     * @brief Главная функция программы
     * 
     * @return int Статус завершения программы
     */
    printf("У вас есть следующие команды на выбор:\n");
    printf("Кодировать в base16 > En_base16\n");
    printf("Кодировать в base32 > En_base32\n");
    printf("Кодировать в base64 > En_base64\n");
    printf("Кодировать в custom_base > En_custom\n");
    printf("Декодировать из base16 > Dec_base16\n");
    printf("Декодировать из base32 > Dec_base32\n");
    printf("Декодировать из base64 > Dec_base64\n");
    printf("Декодировать из custom_base > Dec_custom\n");
    printf("Зашифровать файл > Encrypt\n");
    printf("Расшифровать файл > Decrypt\n");
    printf("Pакодировать в URL > En_URL\n");
    printf("Декодировать из URL > Dec_URL\n");
    printf("Pакодировать в bin > En_bin\n");
    printf("Декодировать из bin > Dec_bin\n");
    printf("Прочитать новый файл > New_file\n");
    printf("Вывести файл с примененными действиями > Print\n");
    printf("Завершение работы > END\n");

    long file_size;
    int size = 0;
    int flag_create = 1;
    int flag_enc16 = 0;
    int flag_enc32 = 0;
    int flag_enc64 = 0;
    int flag_enc = 0;
    int flag_encrypt = 0;
    int flag_url = 0;
    int flag_bin = 0;
    uc *file = read_file(&file_size);

    if (!file) {
        printf("Ошибка чтения файла\n");
        return 1;
    }

    char *file_base = malloc(file_size * 8); // Память для URL-кодированной строки
    if (!file_base) {
        printf("Ошибка выделения памяти для file_base\n");
        free(file); // Освобождаем ранее выделенную память
        return 1;
    }
   
    int MOD;
    char command[50];

    while (1) {
        printf("Выберите действие\n> ");
        if (fgets(command, sizeof(command), stdin) == NULL) break;
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "New_file") == 0) {
            free(file); // Освобождаем старую память перед чтением нового файла
            free(file_base); // Освобождаем память, выделенную под file_base
            file = read_file(&file_size);
            if (!file) {
                printf("Ошибка чтения нового файла\n");
                return 1;
            }
            file_base = malloc(file_size * 8); // Выделяем новую память под file_base
            if (!file_base) {
                printf("Ошибка выделения памяти для file_base\n");
                free(file); // Освобождаем память, выделенную под file
                return 1;
            }
            int flag_create = 1;
            flag_enc16 = 0;
            flag_enc32 = 0;
            flag_enc64 = 0;
            flag_enc = 0;
            flag_enc = 0;
            flag_encrypt = 0;
            flag_url = 0;
            flag_bin = 0;
        }



        if (strcmp(command, "En_base16") == 0){
            if(flag_create && !flag_enc16 && !flag_enc32 && !flag_enc64 && !flag_enc && !flag_encrypt && !flag_url && !flag_bin){
                Encode_to_base16(file_size, &size, file_base, file);
                flag_enc16 = 1;
            }
            else if (!flag_create){
                printf("Пожалуйста, откройте файл\n");
            }
            else if (flag_enc32){
                printf("Файл уже закодирован в base32 декодируйте для начала\n");
            }
            else if (flag_enc64){
                printf("Файл уже закодирован в base64 декодируйте для начала\n");
            }
            else if (flag_enc16){
                printf("Файл уже закодирован в base16 декодируйте его сначала\n");
            }
            else if (flag_encrypt){
                printf("Ваш файл зашифрован!\n");
            }
            else if (flag_url){
                printf("Ваш файл уже закодирован в URL декодируйте его сначала\n");
            }
            else if (flag_bin){
                printf("Файл закодирован в bin\n");
            }
            else{
                printf("Файл уже закодирован в custom_base декодируйте его сначала\n");
            }
        }

        if (strcmp(command, "Dec_base16") == 0){
            if(flag_enc16 && !flag_enc32 && !flag_enc64 && !flag_enc && !flag_encrypt && !flag_url & !flag_bin){
                Decode_from_base16(size, file_base, file);
                size = 0;
                flag_enc16 = 0;
            }
            else if (flag_enc32){
                printf("Ваш код уже закодирван в base32 декодируйте его для начала\n");
            }
            else if (flag_enc64){
                printf("Ваш код уже закодирван в base64 декодируйте его для начала\n");
            }
            else if (flag_enc){
                printf("Ваш код уже закодирван в custom_base декодируйте его для начала\n");
            }
            else if (flag_encrypt){
                printf("Ваш файл зашифрован!\n");
            }
            else if (flag_url){
                printf("Ваш файл уже закодирован в URL декодируйте его сначала\n");
            }
            else if (flag_bin){
                printf("Файл закодирован в bin\n");
            }
            else{
                printf("Пожалуйста, закодируйте файл\n");
            }
        }
        if (strcmp(command, "En_base32") == 0){
            if(flag_create && !flag_enc16 && !flag_enc32 && !flag_enc64 && !flag_enc && !flag_encrypt && !flag_url && !flag_bin){
                Encode_to_base32(&size, file_size, file, file_base);
                flag_enc32 = 1;
            }
            else if (!flag_create){
                printf("Пожалуйста, откройте файл\n");
            }
            else if (flag_enc16){
                printf("Ваш файл уже закодирован в base16 декодируйте его сначала\n");
            }
            else if (flag_enc64){
                printf("Ваш файл уже закодирован в base64 декодируйте его сначала\n");
            }
            else if (flag_enc){
                printf("Ваш файл уже закодирован в custom_base декодируйте его сначала\n");
            }
            else if (flag_encrypt){
                printf("Ваш файл зашифрован!\n");
            }
            else if (flag_url){
                printf("Ваш файл уже закодирован в URL декодируйте его сначала\n");
            }
            else if (flag_bin){
                printf("Файл закодирован в bin\n");
            }
            else{
                printf("Файл уже закодирован в base32 декодируйте его пожалуйста\n");
            }
        }
        if (strcmp(command, "Dec_base32") == 0){
            if(flag_enc32 && !flag_enc16 && !flag_enc64 && !flag_enc && !flag_encrypt && !flag_url && !flag_bin){
                Decode_from_base32(size, file_base, file);
                size = 0;
                flag_enc32 = 0;
            }
            else if (flag_enc16){
                printf("Ваш код уже закодирван в base16 декодируйте его для начала\n");
            }
            else if (flag_enc64){
                printf("Ваш код уже закодирван в base64 декодируйте его для начала\n");
            }
            else if (flag_enc){
                printf("Ваш файл уже закодирован в custom_base декодируйте его сначала\n");
            }
            else if (flag_encrypt){
                printf("Ваш файл зашифрован!\n");
            }
            else if (flag_url){
                printf("Ваш файл уже закодирован в URL декодируйте его сначала\n");
            }
            else if (flag_bin){
                printf("Файл закодирован в bin\n");
            }
            else{
                printf("Пожалуйста, закодируйте файл\n");
            }
        }
        if (strcmp(command, "En_base64") == 0){
            if(flag_create && !flag_enc16 && !flag_enc32 && !flag_enc64 && !flag_enc && !flag_encrypt && !flag_url && !flag_bin){
                Encode_to_base64(&size, file_size, file, file_base);
                flag_enc64 = 1;
            }
            else if (!flag_create){
                printf("Пожалуйста, откройте файл\n");
            }
            else if (flag_enc16){
                printf("Ваш файл уже закодирован в base16 декодируйте его сначала\n");
            }
            else if (flag_enc32){
                printf("Ваш файл уже закодирован в base32 декодируйте его сначала\n");
            }
            else if (flag_enc){
                printf("Ваш файл уже закодирован в custom_base декодируйте его сначала\n");
            }
            else if (flag_encrypt){
                printf("Ваш файл зашифрован!\n");
            }
            else if (flag_url){
                printf("Ваш файл уже закодирован в URL декодируйте его сначала\n");
            }
            else if (flag_bin){
                printf("Файл закодирован в bin\n");
            }
            else{
                printf("Файл уже закодирован в base64 декодируйте его сначала\n");
            }
        }
        if (strcmp(command, "Dec_base64") == 0){
            if(flag_enc64 && !flag_enc16 && !flag_enc32 && !flag_encrypt && !flag_url  && !flag_bin){
                Decode_from_base64(size, file_base, file);
                size = 0;
                flag_enc64 = 0;
            }
            else if (flag_enc16){
                printf("Ваш код уже закодирван в base16 декодируйте его для начала\n");
            }
            else if (flag_enc32){
                printf("Ваш код уже закодирван в base32 декодируйте его для начала\n");
            }
            else if (flag_enc){
                printf("Ваш файл уже закодирован в custom_base декодируйте его сначала\n");
            }
            else if (flag_encrypt){
                printf("Ваш файл зашифрован!\n");
            }
            else if (flag_url){
                printf("Ваш файл уже закодирован в URL декодируйте его сначала\n");
            }
            else if (flag_bin){
                printf("Файл закодирован в bin\n");
            }
            else{
                printf("Пожалуйста, закодируйте файл\n");
            }
        }
        if (strcmp(command, "En_custom") == 0){
            if(flag_create && !flag_enc16 && !flag_enc32 && !flag_enc64 && !flag_enc && !flag_encrypt && !flag_url && flag_bin){
                printf("Введите систему в которую хотите перевести\n");
                printf("В выбранной вами команде вы можете выбрать числа от 20 до 64\n");
                MOD = int_input();
                while(MOD > 64 || MOD < 20){
                    printf("Пожалуйста введите числа из предоставленного диапазона\n");
                    MOD = int_input();
                }
                Custom_base_encode(&size, file_size, MOD, file_base, file);
                flag_enc = 1;
            }
            else if (!flag_create){
                printf("Пожалуйста, откройте файл\n");
            }
            else if (flag_enc16){
                printf("Ваш файл уже закодирован в base16 декодируйте его сначала\n");
            }
            else if (flag_enc32){
                printf("Ваш файл уже закодирован в base32 декодируйте его сначала\n");
            }
            else if (flag_enc){
                printf("Ваш файл уже закодирован в custom_base декодируйте его сначала\n");
            }
            else if (flag_encrypt){
                printf("Ваш файл зашифрован\n");
            }
            else if (flag_url){
                printf("Ваш файл уже закодирован в URL декодируйте его сначала\n");
            }
            else if (flag_bin){
                printf("Файл закодирован в bin\n");
            }
            else{
                printf("Файл уже закодирован в base64 декодируйте его сначала\n");
            }
        }
        if (strcmp(command, "Dec_custom") == 0){
            if(flag_enc && !flag_enc16 && !flag_enc32 && !flag_enc64 && !flag_encrypt && !flag_url && !flag_bin){
                Custom_base_decode(file_size, MOD, file_base, file);
                size = 0;
                flag_enc = 0;
            }
            else if (flag_enc16){
                printf("Ваш код уже закодирван в base16 декодируйте его для начала\n");
            }
            else if (flag_enc32){
                printf("Ваш код уже закодирван в base32 декодируйте его для начала\n");
            }
            else if (flag_enc64){
                printf("Ваш файл уже закодирован в base64 декодируйте его сначала\n");
            }
            else if (flag_encrypt){
                printf("Ваш файл зашифрован\n");
            }
            else if (flag_url){
                printf("Ваш файл уже закодирован в URL декодируйте его сначала\n");
            }
            else if (flag_bin){
                printf("Файл закодирован в bin\n");
            }
            else{
                printf("Пожалуйста, закодируйте файл\n");
            }
        }
        if (strcmp(command, "Encrypt") == 0){
            if(flag_create && !flag_enc16 && !flag_enc32 && !flag_enc64 && !flag_enc && !flag_url && !flag_bin){
                Encryption(&size, file_size, file, file_base);
                flag_encrypt = 1;
            }
            else if (!flag_create){
                printf("Пожалуйста, откройте файл\n");
            }
            else if (flag_enc16){
                printf("Ваш файл уже закодирован в base16 декодируйте его сначала\n");
            }
            else if (flag_enc32){
                printf("Ваш файл уже закодирован в base32 декодируйте его сначала\n");
            }
            else if (flag_enc){
                printf("Ваш файл уже закодирован в custom_base декодируйте его сначала\n");
            }
            else if (flag_url){
                printf("Ваш файл уже закодирован в URL декодируйте его сначала\n");
            }
            else if (flag_bin){
                printf("Файл закодирован в bin\n");
            }
            else{
                printf("Файл уже заифрован\n");
            }
        }
        if (strcmp(command, "Decrypt") == 0){
            if(flag_encrypt && !flag_enc16 && !flag_enc32 && !flag_enc64 && !flag_enc && !flag_url && !flag_bin){
                Decryption(size, file_size, file, file_base);
                size = 0;
                flag_encrypt = 0;
            }
            else if (flag_enc16){
                printf("Ваш код уже закодирван в base16 декодируйте его для начала\n");
            }
            else if (flag_enc32){
                printf("Ваш код уже закодирван в base32 декодируйте его для начала\n");
            }
            else if (flag_enc64){
                printf("Ваш файл уже закодирован в base64 декодируйте его сначала\n");
            }
            else if (flag_enc){
                printf("Ваш файл уже закодирован в base_custom декодируйте его сначала\n");
            }
            else if (flag_url){
                printf("Ваш файл уже закодирован в URL декодируйте его сначала\n");
            }
            else if (flag_bin){
                printf("Файл закодирован в bin\n");
            }
            else{
                printf("Файл не зашифрован\n");
            }
        }
        if (strcmp(command, "En_URL") == 0){
            if(flag_create && !flag_enc16 && !flag_enc32 && !flag_enc64 && !flag_enc && !flag_url && !flag_bin){
                En_URL(&size, file_size, file, file_base);
                flag_url = 1;
            }
            else if (!flag_create){
                printf("Пожалуйста, откройте файл\n");
            }
            else if (flag_enc16){
                printf("Ваш файл уже закодирован в base16 декодируйте его сначала\n");
            }
            else if (flag_enc32){
                printf("Ваш файл уже закодирован в base32 декодируйте его сначала\n");
            }
            else if (flag_enc64){
                printf("Ваш файл уже закодирован в base64 декодируйте его сначала\n");
            }
            else if (flag_enc){
                printf("Ваш файл уже закодирован в custom_base декодируйте его сначала\n");
            }
            else if (flag_bin){
                printf("Файл закодирован в bin\n");
            }
            else if (flag_encrypt){
                printf("Файл зашифрован\n");
            }
        }
        if (strcmp(command, "Dec_URL") == 0){
            if(flag_url && !flag_enc16 && !flag_enc32 && !flag_enc64 && !flag_enc && !flag_encrypt && !flag_bin){
                Dec_URL(file_size, file, file_base);
                size = 0;
                flag_url = 0;
            }
            else if (flag_enc16){
                printf("Ваш код уже закодирван в base16 декодируйте его для начала\n");
            }
            else if (flag_enc32){
                printf("Ваш код уже закодирван в base32 декодируйте его для начала\n");
            }
            else if (flag_enc64){
                printf("Ваш файл уже закодирован в base64 декодируйте его сначала\n");
            }
            else if (flag_enc){
                printf("Ваш файл уже закодирован в base_custom декодируйте его сначала\n");
            }
            else if (flag_encrypt){
                printf("Файл зашифрован\n");
            }
            else if (flag_bin){
                printf("Файл закодирован в bin\n");
            }
            else {
                printf("Файл не закодирован в URL\n");
            }
        }
        if (strcmp(command, "En_bin") == 0){
            if(flag_create && !flag_enc16 && !flag_enc32 && !flag_enc64 && !flag_enc && !flag_url && !flag_bin){
                En_bin(&size, file_size, file, file_base);
                flag_bin = 1;
            }
            else if (!flag_create){
                printf("Пожалуйста, откройте файл\n");
            }
            else if (flag_enc16){
                printf("Ваш файл уже закодирован в base16 декодируйте его сначала\n");
            }
            else if (flag_enc32){
                printf("Ваш файл уже закодирован в base32 декодируйте его сначала\n");
            }
            else if (flag_enc64){
                printf("Ваш файл уже закодирован в base64 декодируйте его сначала\n");
            }
            else if (flag_enc){
                printf("Ваш файл уже закодирован в custom_base декодируйте его сначала\n");
            }
            else if (flag_encrypt){
                printf("Файл зашифрован\n");
            }
            else if (flag_bin){
                printf("Файл закодирован в bin\n");
            }
        }
        if (strcmp(command, "Dec_bin") == 0){
            if(flag_bin && !flag_enc16 && !flag_enc32 && !flag_enc64 && !flag_enc && !flag_encrypt && !flag_url){
                Dec_bin(file_size, file, file_base);
                size = 0;
                flag_bin = 0;
            }
            else if (flag_enc16){
                printf("Ваш код уже закодирван в base16 декодируйте его для начала\n");
            }
            else if (flag_enc32){
                printf("Ваш код уже закодирван в base32 декодируйте его для начала\n");
            }
            else if (flag_enc64){
                printf("Ваш файл уже закодирован в base64 декодируйте его сначала\n");
            }
            else if (flag_enc){
                printf("Ваш файл уже закодирован в base_custom декодируйте его сначала\n");
            }
            else if (flag_encrypt){
                printf("Файл зашифрован\n");
            }
            else if (flag_url){
                printf("Файл закодирован в URL\n");
            }
            else {
                printf("Файл не закодирован в bin\n");
            }
        }
        if (strcmp(command, "Print") == 0){
            if(flag_enc16 || flag_enc32 || flag_enc64 || flag_enc || flag_encrypt || flag_url || flag_bin){
                Write_file(file_base, size);
            }
            else{
                Write_file(file, file_size);
            }
        }

        if (strcmp(command, "END") == 0){
            printf("Программа завершена\n");
            break;
        }
        else{
            printf("Введите корректное действие\n");
        }
    }
    // Закрываем файлы
    free(file);
    free(file_base);
    return 0;
}