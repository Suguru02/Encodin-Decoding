## Encoding/Decoding Library

Простая библиотека для кодирования/декодирования в различных форматах.

## Сборка и запуск

```
gcc -c decode.c encode.c
ar rcs libcode.a encode.o decode.o
gcc main.c -L. -lcode -o myprogram
./myprogram
```

## Использование

```
Находится в main.c.
В нем также прописано интерактивное взаимодействие с пользователем через консоль
```

## Поддерживаемые форматы
- Base16
- Base32
- Base64
- Custom base (берет первые n чисел от 20 до 64 из библиотеки base64 | n выбирает пользователь)
- Bin (двоичный код)
- URL
- Castom (кодирует в алфавит написанный пользователем | чтобы расшифровать нужно также вписать тот же алфавит в том же порядке иначе не расшифрует (в том и была задумка))
