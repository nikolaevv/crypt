#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

int matrix[8][16] = {
    {4, 10, 9, 2, 13, 8, 0, 14, 6, 11, 1, 12, 7, 15, 5, 3},
    {14, 11, 4, 12, 6, 13, 15, 10, 2, 3, 8, 1, 0, 7, 5, 9},
    {5, 8, 1, 13, 10, 3, 4, 2, 14, 15, 12, 7, 6, 0, 9, 11},
    {7, 13, 10, 1, 0, 8, 9, 15, 14, 4, 6, 12, 11, 2, 5, 3},
    {6, 12, 7, 1, 5, 15, 13, 8, 4, 10, 9, 14, 0, 3, 11, 2},
    {4, 11, 10, 0, 7, 2, 1, 13, 3, 6, 8, 5, 9, 12, 15, 14},
    {13, 11, 4, 1, 3, 15, 5, 9, 0, 10, 14, 7, 6, 8, 2, 12},
    {1, 15, 13, 0, 5, 7, 10, 4, 9, 2, 3, 14, 6, 11, 8, 12},
};

int bits_length(char* text) {
    int length = strlen(text);
    int bits_count = length * 16;
    return bits_count;
}

int set_key(long long* key [8], long int nkey [4]) {
    for (int i = 0; i < 8; i++) {

        int rem = 0;

        for (int q = 4; q > 0; q--) {
            rem = (nkey[q] & 1);
            nkey[q] = (nkey[q] >> 1) | (rem << 31);
        }

        key[i] = nkey[0] & 0xFFFFFFFF;
    }

    return key;
}

int get_out(int inright, int key) {
    int out = 0;
    int cell;
    int temp = (inright + key) % (4294967296);
    for (int i = 0; i < 8; i++) {
        cell = (temp >> (4 * i)) & 0b1111;
        // Определение ячейки, сдвиг на 4n бит и выбор пересечения
        out = out | (matrix[i][cell] << (4 * i));
        // Второй бит устанавливается в 0, сдвиг влево на 4n бит
    }
    
    out = ((out >> 21) | (out << 11)) & 0xFFFFFFFF;
    // Пересечение бита и объединения множеств со сдвигами
    return out;
}

int crypt_round(int inleft, int inright, int key) {
    int outleft = inright;
    // Присваиваем левому выходу правые входные данные
    int outright = inleft ^ get_out(inright, key);
    // Исключаем пересечение
    int out [2] = {outleft, outright};
    return outleft, outright;
}

int crypt(int text, int key[8]) {
    int text_left = text >> 32;
    int text_right = text & 0xFFFFFFFF;
    // Разбитие текста с помощью побитового сдвига

    for (int q = 0; q < 24; q++) {

        int crypted = crypt_round(text_left, text_right, key[q % 8]);
        text_left, text_right = crypted;
    }

    for (int q = 0; q < 8; q++) {
        int crypted = crypt_round(text_left, text_right, key[7 - q]);
        text_left, text_right = crypted;
    }

    // 32 цикла с различными ключами

    int hash = (text_left << 32) | text_right;
    printf(hash);
    return hash;
}

_Bool check_hash(int text, char* key, int hash) {
    int nhash = crypt(text, key);
    if (hash == nhash) {
        return 1;
    }
    else {
        return 0;
    }
}

int main(void)
{
    int key[8];
    long long c[4] = {7719590407422755457, 1645157280123808947, 6045707110440092852, 5680108196191993855};
    // Установка ключа из 76 цифр, разбитых в массив на 4 части

    set_key(key, c);
    // Установка ключа
    crypt(("%x", "Привет"), key);

    return 1;
}